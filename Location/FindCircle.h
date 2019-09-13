#pragma once
#include "Calipers.h"
class FindCircle
{
public:
	FindCircle();
	~FindCircle();	
	FindCircle* SetDistThresh(double distThresh) { m_distThresh = distThresh; return this; } 
	double GetDistThresh()const { return m_distThresh; }
	FindCircle* SetmaxIter(const int maxIter) { m_maxIter = maxIter; return this; } 
	int GetMaxIter() const { return m_maxIter; }
	FindCircle* SetRansacEnable(bool b) { m_ransacEnable = b; return this; }
	bool GetRansacEnable() const { return m_ransacEnable; }
	int FindCircle::FitCircle(vector<cv::Point2d> pointsInput, Point3f &circlePara, vector<cv::Point2d> &pointsOutputOK, vector<cv::Point2d> &pointsOutputNG, int countDelete = 0);
private:
	double m_distThresh;
	int m_maxIter;
	bool m_ransacEnable;
	int FindCircle::FitPoints(vector<cv::Point2d> pointsInput, Point3f & circlePara, vector<cv::Point2d> &pointsOutputOK, vector<cv::Point2d> &pointsOutputNG, int countDelete = 0);
	int FindCircle::FilterRansac(vector<cv::Point2d> pointsInput, Point3f & circlePara, vector<cv::Point2d> &pointsOutputOK, vector<cv::Point2d> &pointsOutputNG, int countDelete = 0);
	int FindCircle::getThreePoints(vector<cv::Point2d> pointsInput, vector<Point2d> &chosedPoint, cv::RNG& rng);
	int FindCircle::FitCircleBy3Point(vector<cv::Point2d> pointsInput, vector<Point2d> chosedPoint, double &r, Point2d &center);
	int FindCircle::DeletePoints(vector<Point2d> &selectpointsInputOK, vector<Point2d> &selectpointsInputNG, int countDelete);
};
struct CmpByValue2
{
	bool operator()(const pair<Point2d, double>& lhs, const pair<Point2d, double>& rhs)
	{
		return lhs.second < rhs.second;
	}
};



