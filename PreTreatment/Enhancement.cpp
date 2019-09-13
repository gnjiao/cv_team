#include "PreTreatment\Enhancement.h"

Enhancement::Enhancement()
{
}
Enhancement::~Enhancement()
{
}

int Enhancement::EnhanceEqualizeHist(const cv::Mat srcImage, cv::Mat & dstImage)
{
	if (srcImage.empty())
	{
		return -1;
	}

	equalizeHist(srcImage, dstImage);
	return 0;
}

int Enhancement::EnhanceLaplace(cv::Mat srcImage, cv::Mat & dstImage)
{
	if (srcImage.empty())
	{
		return -1;
	}

	cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

	filter2D(srcImage, dstImage, srcImage.type(), kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
	return 0;
}

int Enhancement::EnhanceGamma(cv::Mat srcImage, cv::Mat & dstImage)
{
	if (srcImage.empty())
	{
		return -1;
	}
	if (srcImage.channels() == 1)
	{
		cv::cvtColor(srcImage, srcImage, CV_GRAY2BGR);
	}
	cv::Mat imageGamma(srcImage.size(), CV_32FC3);
	for (int i = 0; i < srcImage.rows; i++)
	{
		for (int j = 0; j < srcImage.cols; j++)
		{
			imageGamma.at<cv::Vec3f>(i, j)[0] = (srcImage.at<cv::Vec3b>(i, j)[0])*(srcImage.at<cv::Vec3b>(i, j)[0])*(srcImage.at<cv::Vec3b>(i, j)[0]);
			imageGamma.at<cv::Vec3f>(i, j)[1] = (srcImage.at<cv::Vec3b>(i, j)[1])*(srcImage.at<cv::Vec3b>(i, j)[1])*(srcImage.at<cv::Vec3b>(i, j)[1]);
			imageGamma.at<cv::Vec3f>(i, j)[2] = (srcImage.at<cv::Vec3b>(i, j)[2])*(srcImage.at<cv::Vec3b>(i, j)[2])*(srcImage.at<cv::Vec3b>(i, j)[2]);
		}
	}

	normalize(imageGamma, imageGamma, 0, 255, CV_MINMAX);

	convertScaleAbs(imageGamma, imageGamma);

	cv::cvtColor(imageGamma, dstImage, CV_BGR2GRAY);
	return 0;
}

int Enhancement::EnhanceLog(cv::Mat srcImage, cv::Mat & dstImage)
{
	if (srcImage.empty())
	{
		return -1;
	}
	if (srcImage.channels() == 1)
	{
		cv::cvtColor(srcImage, srcImage, CV_GRAY2BGR);
	}
	cv::Mat imageLog(srcImage.size(), CV_32FC3);
	for (int i = 0; i < srcImage.rows; i++)
	{
		for (int j = 0; j < srcImage.cols; j++)
		{
			imageLog.at<cv::Vec3f>(i, j)[0] = log(1 + srcImage.at<cv::Vec3b>(i, j)[0]);
			imageLog.at<cv::Vec3f>(i, j)[1] = log(1 + srcImage.at<cv::Vec3b>(i, j)[1]);
			imageLog.at<cv::Vec3f>(i, j)[2] = log(1 + srcImage.at<cv::Vec3b>(i, j)[2]);
		}
	}

	normalize(imageLog, imageLog, 0, 255, CV_MINMAX);

	convertScaleAbs(imageLog, imageLog);

	cv::cvtColor(imageLog, dstImage, CV_BGR2GRAY);
	return 0;
}