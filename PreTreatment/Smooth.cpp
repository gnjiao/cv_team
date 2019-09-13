#include "PreTreatment\Smooth.h"

Smooth::Smooth()
{
}
Smooth::~Smooth()
{
}

int Smooth::BlurSmooth(const cv::Mat srcImage, cv::Mat &dstImage, int m_KernelSize)
{
	if (srcImage.empty())
	{
		return -1;
	}

	cv::blur(srcImage, dstImage, cv::Size(m_KernelSize, m_KernelSize), cv::Point(-1, -1), cv::BORDER_DEFAULT);
	return 0;
}

int Smooth::GaussianSmooth(const cv::Mat srcImage, cv::Mat &dstImage, int m_KernelSize, double sigmaX, double sigmaY)
{
	if (srcImage.empty())
	{
		return -1;
	}

	cv::GaussianBlur(srcImage, dstImage, cv::Size(m_KernelSize, m_KernelSize), sigmaX, sigmaY, cv::BORDER_DEFAULT);
	return 0;
}

int Smooth::BoxSmooth(const cv::Mat srcImage, cv::Mat &dstImage, int m_KernelSize, bool normalize)
{
	if (srcImage.empty())
	{
		return -1;
	}

	cv::boxFilter(srcImage, dstImage, -1, cv::Size(m_KernelSize, m_KernelSize), cv::Point(-1, -1), normalize, cv::BORDER_DEFAULT);
	return 0;
}

int Smooth::BilateralSmooth(cv::Mat srcImage, cv::Mat &dstImage, double sigmaColor, double sigmaSpace, int d)
{
	if (srcImage.empty())
	{
		return -1;
	}

	cv::bilateralFilter(srcImage, dstImage, d, sigmaColor, sigmaSpace, cv::BORDER_DEFAULT);
	return 0;
}

int Smooth::MedianSmooth(const cv::Mat srcImage, cv::Mat &dstImage, int m_KernelSize)
{
	if (srcImage.empty())
	{
		return -1;
	}
	cv::medianBlur(srcImage, dstImage, m_KernelSize);
	return 0;
}