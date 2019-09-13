#include "Acquisition.h"


Acquisition * Acquisition::SaveReferenceMat(string fileName)
{
	cv::imwrite(fileName, m_referenceMat);
	return this;
}
cv::Mat Acquisition::LoadReferenceMat(string fileName)
{
	this->m_referenceMat = cv::imread(fileName);
	return this->m_referenceMat;
}
cv::Mat Acquisition::FromFile(string fileName)
{
	this->m_currentMat = cv::imread(fileName, CV_8UC1);
	return this->m_currentMat;
}
cv::Mat Acquisition::FromCamera()
{
	unsigned char* image;
	int imageSize, h, w;
	cv::Mat src;
	return src;
}