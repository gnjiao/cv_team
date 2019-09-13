#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <Eigen\Dense>
#include <Eigen\LU>

using namespace std;
using namespace Eigen;

class Threshold
{
public:
	enum //thresholdFixType
	{
		THRESH_BINARY = 0,
		THRESH_BINARY_INV = 1,
		THRESH_OTSU = 8,
		THRESH_TRIANGLE = 16
	};
	enum //AutothresholdType
	{
		ADAPTIVE_THRESH_MEAN_C = 0,
		ADAPTIVE_THRESH_GAUSSIAN_C = 1
	};
public:
	Threshold();
	~Threshold();
	int Threshold::ThresholdFix(cv::Mat srcImage, cv::Mat &dstImage, int thresholdValue, int maxValue, int thresholdType);//4??
	int Threshold::ThresholdAuto(cv::Mat srcImage, cv::Mat &dstImage, int maxValue, int autoThresholdType, int thresholdTypeBinary, int blockSize = 5, double C = 10);//?2
	int Threshold::ThresholdSegment(cv::Mat srcImage, cv::Mat &dstImage, int baseValue, int maxValue);
	int Threshold::ThresholdTwoPeak(cv::Mat srcImage, cv::Mat & dstImage, int maxValue, int thresholdTypeBinary);//?2
private:
	bool Threshold::TwoPeakCheck(cv::Mat histogram);
	int Threshold::GetThresholdValue(cv::Mat histogram, int &thresholdValue);
};
