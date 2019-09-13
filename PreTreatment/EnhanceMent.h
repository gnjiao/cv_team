#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <Eigen\Dense>
#include <Eigen\LU>

using namespace std;

class Enhancement
{
public:
	Enhancement();
	~Enhancement();
	int Enhancement::EnhanceEqualizeHist(const cv::Mat srcImage, cv::Mat &dstImage);
	int Enhancement::EnhanceLaplace(cv::Mat srcImage, cv::Mat &dstImage);
	int Enhancement::EnhanceGamma(cv::Mat srcImage, cv::Mat &dstImage);
	int Enhancement::EnhanceLog(cv::Mat srcImage, cv::Mat &dstImage);
private:
};