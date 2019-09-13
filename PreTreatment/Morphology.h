#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <Eigen\Dense>
#include <Eigen\LU>

using namespace std;

class Morphology
{
public:
	Morphology();
	~Morphology();
	Morphology* SetStructurElementShape(int StructurElementShape) { m_StructElementShape = StructurElementShape; return this; }
	int GetStructurElementShape()const { return m_StructElementShape; }

	int Morphology::Dilate(cv::Mat srcImage, cv::Mat &dstImage, int structElementSize = 1, int iteration = 1);

	int Morphology::Erode(cv::Mat srcImage, cv::Mat &dstImage, int structElementSize = 1, int iteration = 1);

	int Morphology::MorphologyOperator(cv::Mat srcImage, cv::Mat &dstImage, cv::MorphTypes m_MorphTypes,
		int structElementSize = 1, int iteration = 1);
private:

	int m_StructElementShape;

};