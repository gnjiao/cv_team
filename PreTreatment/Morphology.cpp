#include "PreTreatment\Morphology.h"

Morphology::Morphology()
{
	m_StructElementShape = cv::MorphShapes::MORPH_RECT;
	//m_StructurAnchor = cv::Point(-1, -1);
}
Morphology::~Morphology()
{
}

int Morphology::Dilate(cv::Mat srcImage, cv::Mat &dstImage, int structElementSize, int iteration)
{
	if (srcImage.empty())
	{
		return -1;
	}
	cv::Mat element = cv::getStructuringElement(m_StructElementShape, cv::Size(2 * structElementSize + 1, 2 * structElementSize + 1), cv::Point(structElementSize, structElementSize));

	cv::dilate(srcImage, dstImage, element, cv::Point(-1, -1), iteration, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
	return 0;
}
int Morphology::Erode(cv::Mat srcImage, cv::Mat &dstImage, int m_StructElementSize, int iteration)
{
	if (srcImage.empty())
	{
		return -1;
	}
	cv::Mat element = cv::getStructuringElement(m_StructElementShape, cv::Size(2 * m_StructElementSize + 1, 2 * m_StructElementSize + 1), cv::Point(m_StructElementSize, m_StructElementSize));
	cv::erode(srcImage, dstImage, element, cv::Point(-1, -1), iteration, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
	return 0;
}

int Morphology::MorphologyOperator(cv::Mat srcImage, cv::Mat &dstImage, cv::MorphTypes m_MorphTypes, int m_StructElementSize, int iteration)
{
	if (srcImage.empty())
	{
		return -1;
	}
	cv::Mat element = cv::getStructuringElement(m_StructElementShape, cv::Size(2 * m_StructElementSize + 1, 2 * m_StructElementSize + 1), cv::Point(m_StructElementSize, m_StructElementSize));
	cv::morphologyEx(srcImage, dstImage, m_MorphTypes, element, cv::Point(-1, -1), iteration, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
	return 0;
}