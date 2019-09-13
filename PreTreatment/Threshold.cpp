#include "PreTreatment\Threshold.h"

Threshold::Threshold()
{
}
Threshold::~Threshold()
{
}
// m_thresholdType:
// THRESH_BINARY = 0,
// THRESH_BINARY_INV = 1,
// THRESH_OTSU = 8,
// THRESH_TRIANGLE = 16,
int Threshold::ThresholdFix(cv::Mat srcImage, cv::Mat &dstImage, int thresholdValue, int maxValue, int thresholdType)
{
	threshold(srcImage, dstImage, thresholdValue, maxValue, thresholdType);
	return 0;
}

int Threshold::ThresholdAuto(cv::Mat srcImage, cv::Mat &dstImage, int maxValue, int autoThresholdType, int thresholdTypeBinary, int blockSize, double C)
{
	adaptiveThreshold(srcImage, dstImage, maxValue, autoThresholdType, thresholdTypeBinary, blockSize, C);
	return 0;
}

int Threshold::ThresholdSegment(cv::Mat srcImage, cv::Mat &dstImage, int baseValue, int highValue)
{
	if (srcImage.channels() == 1)
	{
		dstImage = srcImage;
	}
	else
	{
		cv::cvtColor(srcImage, dstImage, CV_BGR2GRAY, CV_8UC1);
	}
	dstImage = srcImage.clone();
	for (int i = 0; i<dstImage.rows; i++)
	{
		uchar* data = dstImage.ptr<uchar>(i);
		for (int j = 0; j<dstImage.cols; j++)
		{
			if (data[j]<baseValue || data[j]>highValue)
			{
				data[j] = 0;
			}
			else data[j] = 255;
		}
	}
	return 0;
}
int Threshold::ThresholdTwoPeak(cv::Mat srcImage, cv::Mat & dstImage, int maxValue, int thresholdTypeBinary)
{
	// cout << srcImage.type() << endl;
	const int channels[1] = { 0 };
	const int histSize[1] = { 256 };
	float hranges[2] = { 0,256 };
	const float* ranges[1] = { hranges };
	cv::MatND hist;
	calcHist(&srcImage, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
	GaussianBlur(hist, hist, cv::Size(5, 5), 0, 0);
	transpose(hist, hist);

	int thresholdValue;
	GetThresholdValue(hist, thresholdValue);

	threshold(srcImage, dstImage, thresholdValue, maxValue, thresholdTypeBinary);
	return 0;
}
int Threshold::GetThresholdValue(cv::Mat histogram, int &thresholdValue)
{
	int Iter = 0;
	cv::Mat histogramC = histogram;
	cv::Mat histogramCC;

	while (TwoPeakCheck(histogram) == false)
	{

		for (int nrows = 0; nrows < histogram.rows; nrows++)
		{
			float * dataC = histogramC.ptr<float>(nrows);
			float * dataCC = histogramCC.ptr<float>(nrows);

			dataCC[0] = (dataC[0] + dataC[0] + dataC[1]) / 3;
			for (int ncols = 1; ncols < histogram.cols; ncols++)
			{
				dataCC[ncols] = (dataC[ncols - 1] + dataC[ncols] + dataC[ncols + 1]) / 3;
			}

			dataCC[255] = (dataC[254] + dataC[255] + dataC[255]) / 3;
		}
		histogramC = histogramCC;
		Iter++;

		if (Iter >= 1000)
			break;
	}

	bool Peakfound = false;
	thresholdValue = 0;
	for (int nrows = 0; nrows < histogram.rows; nrows++)
	{
		float * dataCC = histogramCC.ptr<float>(nrows);
		for (int ncols = 1; ncols < histogram.cols; ncols++)
		{
			if (dataCC[ncols - 1] < dataCC[ncols] && dataCC[ncols + 1] < dataCC[ncols])
			{
				Peakfound = true;
			}
			if (Peakfound == true && dataCC[ncols - 1] >= dataCC[ncols] && dataCC[ncols + 1] >= dataCC[ncols])
			{
				thresholdValue = ncols - 1;
			}
		}
	}

	return -1;
}

bool Threshold::TwoPeakCheck(cv::Mat histogram)
{

	int count = 0;
	for (int nrows = 0; nrows < histogram.rows; nrows++)
	{
		float * data = histogram.ptr<float>(nrows);
		for (int ncols = 1; ncols < histogram.cols; ncols++)
		{
			//cout << " hist value " << ncols << "= " << data[ncols] << endl;
			if (data[ncols - 1] < data[ncols] && data[ncols + 1] < data[ncols])
			{
				count++;
				if (count > 2)
					return false;
			}
		}
	}
	if (count == 2)
		return true;
	else
		return false;
}