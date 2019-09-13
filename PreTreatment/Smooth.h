#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <Eigen\Dense>
#include <Eigen\LU>

using namespace std;

class Smooth
{
public:
	Smooth();
	~Smooth();

	int Smooth::BlurSmooth(const cv::Mat srcImage, cv::Mat &dstImage, int m_KernelSize = 3);

	int Smooth::GaussianSmooth(const cv::Mat srcImage, cv::Mat &dstImage, int m_KernelSize = 3, double sigmaX = 0, double sigmaY = 0);

	int Smooth::BoxSmooth(const cv::Mat srcImage, cv::Mat &dstImage, int m_KernelSize = 3, bool normalize = true);

	int Smooth::BilateralSmooth(const cv::Mat srcImage, cv::Mat &dstImage, double sigmaColor = 25 * 2, double sigmaSpace = 25 / 2, int d = 25);

	int Smooth::MedianSmooth(const cv::Mat srcImage, cv::Mat &dstImage, int m_KernelSize = 3);

};