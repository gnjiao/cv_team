#include "Blob.h"
#include <algorithm>

using namespace SIMPLEBLOB_SPACE;

bool SIMPLEBLOB_SPACE::SimpleBlob::checkroi(cv::Rect roi, cv::Mat & m)
{
	if (roi.x < 0)
		return false;
	if (roi.x + roi.width>m.cols)
		return false;
	if (roi.y < 0)
		return false;
	if (roi.y + roi.height>m.rows)
		return false;
	return true;
}
bool SimpleBlob::compare_are(CResult A, CResult B)
{
	return A.area > B.area;
}
uchar SIMPLEBLOB_SPACE::SimpleBlob::PointInPloygonTest(std::vector<cv::Point2i> contour, cv::Mat image)
{
	cv::Point2i test;
	uchar width = 2;
	uchar result = 0;
	cv::Point2i p;
	for (int k = 0; k<contour.size(); ++k)
	{
		p = contour.at(k);
		test.x = p.x + width;
		test.y = p.y;
		if (1 == pointPolygonTest(contour, test, false))
		{
			result = image.at<uchar>(test.y, test.x);
			return result;
		}
		test.x = p.x - width;
		test.y = p.y;
		if (1 == pointPolygonTest(contour, test, false))
		{
			result = image.at<uchar>(test.y, test.x);
			return result;
		}
		test.x = p.x;
		test.y = p.y + width;
		if (1 == pointPolygonTest(contour, test, false))
		{
			result = image.at<uchar>(test.y, test.x);
			return result;
		}
		test.x = p.x;
		test.y = p.y - width;
		if (1 == pointPolygonTest(contour, test, false))
		{
			result = image.at<uchar>(test.y, test.x);
			return result;
		}
	}
	result = image.at<uchar>(contour.at(0).y, contour.at(0).x);
	return result;
}
bool SIMPLEBLOB_SPACE::SimpleBlob::InFeature(feature & test, double &ration)
{
	if (Mode::INSIDE == test.mode)
	{
		if (ration < test.minValue || ration >test.maxValue)
		{
			return false;
		}
	}
	else
	{
		if (ration >= test.minValue && ration <= test.maxValue)
		{
			return false;
		}
	}
	return true;
}
Params::Params()
{
	thresholdStep = 10;
	threshold = 50;
	maxThreshold = 220;
	minRepeatability = 2;
	minDistBetweenBlobs = 10;
	filterByColor = false;
	blobColor = 0;
	filterByArea = true;
	minArea = 500;
	maxArea = 800000;
	filterByCircularity = false;
	minCircularity = 0.0f;
	maxCircularity = 1.0f;
	filterByInertia = false;
	minInertiaRatio = 0.0f;
	maxInertiaRatio = 1.0f;
	filterByConvexity = false;
	minConvexity = 0.0f;
	maxConvexity = 1.0f;
	Area.name = "Area";
	Area.inuse = true;
	Area.minValue = 1000;
	Area.maxValue = 50000;
	Circularity.name = "Circularity";
	Circularity.inuse = false;
	Inertia.name = "Inertia";
	Inertia.inuse = false;
	Inertia.minValue = 0;
	Inertia.maxValue = 1;
	Convexity.name = "Convexity";
	Convexity.inuse = false;
	Convexity.minValue = 0;
	Convexity.maxValue = 1;
	Rectangularity.name = "Rectangularity";
	Rectangularity.inuse = false;
	Rectangularity.minValue = 0;
	Rectangularity.maxValue = 1;
	BolbWide.name = "Wide";
	BolbWide.inuse = false;
	BolbWide.minValue = 1;
	BolbWide.maxValue = 1000;
	BlobHeight.name = "Height";
	BlobHeight.inuse = false;
	BlobHeight.minValue = 1;
	BlobHeight.maxValue = 1000;
	Compactness.name = "Compactness";
	Compactness.inuse = false;
	Compactness.minValue = 1;
	Compactness.maxValue = 1000;
	Conlength.name = "Conlength";
	Conlength.inuse = false;
	Conlength.minValue = 1;
	Conlength.maxValue = 10000;
	museroi = false;
	mroi_LeftTopX = 0;
	mroi_LeftTopY = 0;
	mwidth = 100;
	mheight = 100;
	center_x = center_y = 0;
}
void SimpleBlob::findBlobs(cv::InputArray Image1)
{
	using namespace cv;
	useroi = false;
	cv::Mat image = Image1.getMat().clone();
	if (1 > image.rows)
		return;
	threshold(image, image, m_params.threshold, 255, THRESH_BINARY);
	m_result.clear();
	std::vector < std::vector<cv::Point> > contours;
	cv::Mat tmpBinaryImage = image;
	cv::findContours(tmpBinaryImage, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
	//
	for (size_t contourIdx = 0; contourIdx < contours.size(); contourIdx++)
	{
		CResult m_temp_result;
		cv::Moments moms = cv::moments(cv::Mat(contours[contourIdx]));
		if (m_params.Area.inuse)
		{
			double area = moms.m00;
			if (!InFeature(m_params.Area, area))
				continue;
			m_temp_result.area = area;
		}
		if (m_params.filterByColor)
		{
			/*if (image.at<uchar>(cvRound(m_temp_result.location.y), cvRound(m_temp_result.location.x)) != m_params.blobColor)*/
			if (PointInPloygonTest(contours[contourIdx], tmpBinaryImage) != m_params.blobColor)
				continue;
		}
		double perimeter = -1;
		if (m_params.Circularity.inuse || m_params.Compactness.inuse || m_params.Conlength.inuse)
		{
			perimeter = cv::arcLength(cv::Mat(contours[contourIdx]), true);
		}
		if (m_params.Circularity.inuse)
		{
			double area = moms.m00;
			double ratio = 4 * CV_PI * area / (perimeter * perimeter);
			if (ratio > 1)
				ratio = 1;
			if (!InFeature(m_params.Circularity, ratio))
				continue;
			m_temp_result.Circularity = ratio;
		}
		if (m_params.Compactness.inuse)
		{
			double area = moms.m00;
			double ration = perimeter*perimeter / (4 * area*CV_PI);
			if (ration < 1)
				ration = 1;
			if (!InFeature(m_params.Compactness, ration))
				continue;
			m_temp_result.Compactness = ration;
		}
		if (m_params.Conlength.inuse)
		{
			if (!InFeature(m_params.Conlength, perimeter))
				continue;
			m_temp_result.Conlength = perimeter;
		}
		if (m_params.Inertia.inuse)
		{
			double denominator = std::sqrt(std::pow(2 * moms.mu11, 2) + std::pow(moms.mu20 - moms.mu02, 2));
			const double eps = 1e-2;
			double ratio;
			if (denominator > eps)
			{
				double cosmin = (moms.mu20 - moms.mu02) / denominator;
				double sinmin = 2 * moms.mu11 / denominator;
				double cosmax = -cosmin;
				double sinmax = -sinmin;
				double imin = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmin - moms.mu11 * sinmin;
				double imax = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmax - moms.mu11 * sinmax;
				ratio = imin / imax;
			}
			else
			{
				ratio = 1;
			}
			if (!InFeature(m_params.Inertia, ratio))
				continue;
			m_temp_result.Inertia = ratio;
		}
		if (m_params.Convexity.inuse)
		{
			std::vector < Point > hull;
			convexHull(Mat(contours[contourIdx]), hull);
			double area = contourArea(Mat(contours[contourIdx]));
			double hullArea = contourArea(Mat(hull));
			double ratio = area / hullArea;
			if (!InFeature(m_params.Convexity, ratio))
				continue;
			m_temp_result.Convexity = ratio;
		}
		if (m_params.Rectangularity.inuse)
		{
			double area = moms.m00;
			Point2f center;
			float ridus = 0;
			cv::RotatedRect rect = minAreaRect((contours[contourIdx]));
			double ratio = area / (rect.size.width*rect.size.height);
			if (ratio > 1)
				ratio = 1;
			if (!InFeature(m_params.Rectangularity, ratio))
				continue;
			m_temp_result.Rectangularity = ratio;
		}
		cv::Rect boundingrect;
		if (m_params.BolbWide.inuse || m_params.BlobHeight.inuse)
			boundingrect = cv::boundingRect(contours[contourIdx]);
		if (m_params.BolbWide.inuse)
		{
			double wide = boundingrect.width;
			if (!InFeature(m_params.BolbWide, wide))
				continue;
			m_temp_result.wide = wide;
		}
		if (m_params.BlobHeight.inuse)
		{
			double height = boundingrect.height;
			if (!InFeature(m_params.BlobHeight, height))
				continue;
			m_temp_result.heiht = height;
		}
		if (moms.m00 == 0.0)
			continue;
		m_temp_result.location = Point2d(moms.m10 / moms.m00, moms.m01 / moms.m00);
		//compute blob radius
		// {
		// std::vector<double> dists;
		// for (size_t pointIdx = 0; pointIdx < contours[contourIdx].size(); pointIdx++)
		// {
		// Point2d pt = contours[contourIdx][pointIdx];
		// dists.push_back(norm(m_temp_result.location - pt));
		// }
		// std::sort(dists.begin(), dists.end());
		// m_temp_result.radius = (dists[(dists.size() - 1) / 2] + dists[dists.size() / 2]) / 2.;
		// }
		m_temp_result.m_contours = contours[contourIdx];
		m_result.push_back(m_temp_result);
	}
	std::sort(m_result.begin(), m_result.end(), compare_are);
}
void SIMPLEBLOB_SPACE::SimpleBlob::findBlobs(cv::InputArray Image, cv::Rect roi)
{
	mroi = roi;
	cv::Mat pimage = Image.getMat();
	if (1 > pimage.rows)
		return;
	if (!checkroi(roi, pimage))
	{
		return;
	}
	cv::Mat smallmat = cv::Mat(Image.getMat(), roi);
	findBlobs(smallmat);
	std::vector<CResult>::iterator it = m_result.begin();
	for (; it <m_result.end(); it++)
	{
		(*it).location.x += roi.x;
		(*it).location.y += roi.y;
	}
	useroi = true;
}
SimpleBlob* SimpleBlob::set_params(Params para)
{
	m_params = para;
	return this;
}
SimpleBlob::SimpleBlob(const Params &parameters)
{
	useroi = false;
	m_params = parameters;
}
SimpleBlob::SimpleBlob()
{
	useroi = false;
}
void SimpleBlob::Draw_results(cv::Mat &picture, int index/* =-1 */)
{
	if (m_result.empty())
		return;
	cv::Scalar color(0, 255, 0);
	std::vector<std::vector<cv::Point> > contours;
	cv::Point roioffset(0, 0);
	if (useroi)
	{
		roioffset.x = mroi.x;
		roioffset.y = mroi.y;
	}
	if (0 <= index&&m_result.size()>index)
	{
		contours.push_back(m_result[index].m_contours);
		drawMarker(picture, m_result[index].location, color);
		cv::drawContours(picture, contours, 0, color, cv::FILLED, cv::LINE_8, cv::noArray(), 2147483647, roioffset);
	}
	else
	{
		for each (auto &var in m_result)
		{
			contours.clear();
			contours.push_back(var.m_contours);
			drawMarker(picture, var.location, color);
			cv::drawContours(picture, contours, -1, color, cv::FILLED, cv::LINE_8, cv::noArray(), 2147483647, roioffset);
		}
	}
}
Params SimpleBlob::get_params() const
{
	return m_params;
}
SIMPLEBLOB_SPACE::feature::feature()
{
	minValue = 0;
	maxValue = 1;
	mode = Mode::INSIDE;
	inuse = false;
}