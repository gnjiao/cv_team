#pragma once

#include "Calipers.h"

using namespace cv;

class FindLine

{

public:

	FindLine();

	~FindLine();



	enum

	{

		RansacFilter = 0,

		HuberFilter = 1,

		TukeyFilter = 2,

	};



	//FindLine* SetDistance(int distance) { m_distance = distance; return this; }

	//int GetDistance()const { return m_distance; }



	FindLine* SetFilterType(int filterType) { m_filterType = filterType; return this; }

	int GetFilterType() const { return m_filterType; }



	FindLine* SetDistThresh(double distThresh) { m_distThresh = distThresh; return this; }

	double GetDistThresh()const { return m_distThresh; }



	FindLine* SetMaxIter(const int maxIter) { m_maxIter = maxIter; return this; }

	int GetMaxIter() const { return m_maxIter; }



	int FindLine::FilterPoints(const vector<cv::Point2d> pointsInput, vector<cv::Point2d> &pointsOutputOK, vector<cv::Point2d> &pointsOutputNG, int countDelete = 0);

	void FindLine::FitPoints(vector<cv::Point2d> pointsInput, cv::Vec4f &lineOut);



private:



	//int m_distance;

	int m_filterType;

	double m_distThresh;

	int m_maxIter;



	int FindLine::DeletePoints(vector<Point2d> &selectpointsInputOK, vector<Point2d> &selectpointsInputNG, int countDelete);

	float FindLine::GetDistancePoint2LineTukey(Vec4f line, Point2d point);

	float FindLine::GetDistancePoint2LineHuber(Vec4f line, Point2d point);

};



struct CmpByValue

{

	bool operator()(const pair<Point2d, double>& lhs, const pair<Point2d, double>& rhs)

	{

		return lhs.second < rhs.second;

	}

};