#pragma once

#include <opencv2\core\core.hpp>

#include <opencv2\highgui\highgui.hpp>

#include <opencv2\imgproc\imgproc.hpp>

#include <iostream>

#include <math.h>

#include <map>

#include <Windows.h>

#include <Eigen\Dense>

#include <Eigen\LU>



using namespace cv;

using namespace std;

using namespace Eigen;



class Calipers

{

public:

	Calipers() : m_polarProperty(0), m_resultType(0), m_threshold_value(30) {}

	~Calipers() {}



	enum polarProperty

	{

		WhiteToBlack = 0,

		BlackToWhite = 1,

		TwoPolarisation = 2,

	};



	enum resultType

	{

		FirstLine = 0,

		BestLine = 1,

		FinalLine = 2,

	};




	Calipers* SetPolarProperty(int polarProperty) { m_polarProperty = polarProperty; return this; }

	int GetPolarProperty()const { return m_polarProperty; }



	Calipers* SetResultType(int resultType) { m_resultType = resultType; return this; }

	int GetResultType()const { return m_resultType; }



	Calipers* SetThresholdValue(int threshold_value) { m_threshold_value = threshold_value; return this; }

	int GetThresholdValue()const { return m_threshold_value; }



	Calipers* SetImage(Mat srcImage)

	{

		if (srcImage.channels() == 1)

		{

			m_srcImage = srcImage;

		}

		else

		{

			cvtColor(srcImage, m_srcImage, CV_BGR2GRAY, CV_8UC1);

		}

		return this;

	}

	Mat GetImage()const { return m_srcImage; }



	int Calipers::Caliper(vector<Point2d> RectConers, vector<Point2d>& pointsOut);





private:

	Mat m_srcImage;

	int m_polarProperty;

	int m_resultType;

	int m_threshold_value;



	int Calipers::FindLinePosition(Eigen::MatrixXd differences, const Point2d pointA,

		const Point2d pointB, const Point2d pointD, vector<Point2d>& pointsOut);



	int Calipers::FindLinePosition2(Eigen::MatrixXd differencesP, Eigen::MatrixXd differencesN, const Point2d pointA,

		const Point2d pointB, const Point2d pointD, vector<Point2d>& pointsOut);

};

