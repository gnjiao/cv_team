#include "FindLine.h"
FindLine::FindLine()
{
	m_filterType = 0;
	m_maxIter = 2000;
	m_distThresh = 5;
}
FindLine::~FindLine()
{
}
int FindLine::FilterPoints(const vector<cv::Point2d> pointsInput, vector<cv::Point2d> &pointsOutputOK, vector<cv::Point2d> &pointsOutputNG, int countDelete)
{
	vector<cv::Point2d> selectpointsInputOK;
	vector<cv::Point2d> selectpointsInputNG;
	int chosedPointNum[2];
	int k = m_maxIter;
	vector<cv::Point2d> tmpSelectpointsInputOK;
	vector<cv::Point2d> tmpSelectpointsInputNG;
	double a, b, c;
	double dist;
	vector<cv::Point2d> pointsTemp;
	vector<cv::Point2d> pointsTempNG;
	float distanceTemp = 0;
	Vec4f linePoints;
	if (pointsInput.empty())
	{
		return 1;
	}
	if (m_maxIter <= 0)
	{
		return 1;
	}
	if (m_distThresh <= 0)
	{
		return 1;
	}
	if (m_filterType == 0)
	{
		cv::RNG rng((uint64)-1);
		for (int i = 0; i < m_maxIter; i++)
		{
			//chosedPointNum[0] = rand() % (pointsInput.size());
			//chosedPointNum[1] = rand() % (pointsInput.size());
			chosedPointNum[0] = rng.uniform(0, pointsInput.size());
			chosedPointNum[1] = rng.uniform(0, pointsInput.size());
			//cout << chosedPointNum[0] << " " << chosedPointNum[1] << endl;
			if ((pointsInput[chosedPointNum[0]].x == pointsInput[chosedPointNum[1]].x) &&
				(pointsInput[chosedPointNum[0]].y == pointsInput[chosedPointNum[1]].y))
			{
				i--;
				continue;
			}
			//ax+by+c=0
			a = pointsInput[chosedPointNum[1]].y - pointsInput[chosedPointNum[0]].y;
			b = pointsInput[chosedPointNum[0]].x - pointsInput[chosedPointNum[1]].x;
			c = (pointsInput[chosedPointNum[1]].x * pointsInput[chosedPointNum[0]].y -
				pointsInput[chosedPointNum[0]].x * pointsInput[chosedPointNum[1]].y);
			tmpSelectpointsInputOK.clear();
			tmpSelectpointsInputNG.clear();
			for (int j = 0; j < pointsInput.size(); j++)
			{
				dist = abs(a * pointsInput[j].x + b * pointsInput[j].y + c) / sqrt(a * a + b * b);
				if (dist < m_distThresh)
				{
					tmpSelectpointsInputOK.push_back(pointsInput[j]);
				}
				else
				{
					tmpSelectpointsInputNG.push_back(pointsInput[j]);
				}
			}
			if (tmpSelectpointsInputOK.size() > selectpointsInputOK.size())
			{
				k = log(0.0001) / log(1 - pow((double)tmpSelectpointsInputOK.size() / pointsInput.size(), 2));
				//cout << k << "****" << endl;
				selectpointsInputOK.clear();
				selectpointsInputNG.clear();
				selectpointsInputOK.assign(tmpSelectpointsInputOK.begin(), tmpSelectpointsInputOK.end());
				selectpointsInputNG.assign(tmpSelectpointsInputNG.begin(), tmpSelectpointsInputNG.end());
			}
			if (i > k)
				break;
		}
		if (selectpointsInputNG.size()<countDelete)
		{
			int tempNum = countDelete - selectpointsInputNG.size();
			DeletePoints(selectpointsInputOK, selectpointsInputNG, tempNum);
		}
		pointsOutputOK.clear();
		pointsOutputNG.clear();
		pointsOutputOK = selectpointsInputOK;
		pointsOutputNG = selectpointsInputNG;
	}
	if (m_filterType == 1)
	{
		pointsTemp.assign(pointsInput.begin(), pointsInput.end());
		for (;;)
		{
			fitLine(pointsTemp, linePoints, CV_DIST_HUBER, 0, 0.01, 0.01);
			for (int i = 0; i < pointsTemp.size(); i++)
			{
				distanceTemp = GetDistancePoint2LineHuber(linePoints, pointsTemp[i]);
				if (distanceTemp <= m_distThresh)
				{
					tmpSelectpointsInputOK.push_back(pointsTemp[i]);
				}
				else
				{
					tmpSelectpointsInputNG.push_back(pointsTemp[i]);
				}
			}
			if (pointsTemp.size() == tmpSelectpointsInputOK.size() || tmpSelectpointsInputOK.size() == 0)
				break;
			pointsTemp.clear();
			pointsTemp = tmpSelectpointsInputOK;
			pointsTempNG = tmpSelectpointsInputNG;
			tmpSelectpointsInputOK.clear();
			tmpSelectpointsInputNG.clear();
		}
		if (pointsTempNG.size()<countDelete)
		{
			int tempNum = countDelete - pointsTempNG.size();
			DeletePoints(pointsTemp, pointsTempNG, tempNum);
		}
		pointsOutputOK.clear();
		pointsOutputNG.clear();
		pointsOutputOK = pointsTemp;
		pointsOutputNG = pointsTempNG;
	}
	if (m_filterType == 2)
	{
		pointsTemp.assign(pointsInput.begin(), pointsInput.end());
		for (;;)
		{
			fitLine(pointsTemp, linePoints, CV_DIST_L12, 10, 0.01, 0.01);
			for (int i = 0; i < pointsTemp.size(); i++)
			{
				distanceTemp = GetDistancePoint2LineTukey(linePoints, pointsTemp[i]);
				if (distanceTemp <= m_distThresh)
				{
					tmpSelectpointsInputOK.push_back(pointsTemp[i]);
				}
				else
				{
					tmpSelectpointsInputNG.push_back(pointsTemp[i]);
				}
			}
			if (pointsTemp.size() == tmpSelectpointsInputOK.size() || tmpSelectpointsInputOK.size() == 0)
				break;
			pointsTemp.clear();
			pointsTemp = tmpSelectpointsInputOK;
			pointsTempNG = tmpSelectpointsInputNG;
			tmpSelectpointsInputOK.clear();
			tmpSelectpointsInputNG.clear();
		}
		if (pointsTempNG.size()<countDelete)
		{
			int tempNum = countDelete - pointsTempNG.size();
			DeletePoints(pointsTemp, pointsTempNG, tempNum);
		}
		pointsOutputOK.clear();
		pointsOutputNG.clear();
		pointsOutputOK = pointsTemp;
		pointsOutputNG = pointsTempNG;
	}
	return 0;
}
void FindLine::FitPoints(vector<cv::Point2d> pointsInput, Vec4f & lineOut)
{
	fitLine(pointsInput, lineOut, CV_DIST_L12, 0, 0.01, 0.01);
	return;
}
float FindLine::GetDistancePoint2LineTukey(Vec4f line, Point2d point)
{
	float a, b, c, distance;
	a = line[1] / line[0];
	b = -1;
	c = line[3] - a*line[2];
	distance = abs(a * point.x + b * point.y + c) / sqrt(a * a + b * b);
	return distance;
}
float FindLine::GetDistancePoint2LineHuber(Vec4f line, Point2d point)
{
	float a, b, c, distance;
	a = line[1] / line[0];
	b = -1;
	c = line[3] - a*line[2];
	distance = abs(point.y - (a*point.x + c) / b);
	return distance;
}
int FindLine::DeletePoints(vector<Point2d> &selectpointsInputOK, vector<Point2d> &selectpointsInputNG, int countDelete)
{
	Vec4f linePoints;
	vector<pair<Point2d, double>> vec;
	double distance = 0;
	cv::fitLine(selectpointsInputOK, linePoints, CV_DIST_L2, 0, 1e-2, 1e-2);
	for each (auto point in selectpointsInputOK)
	{
		distance = GetDistancePoint2LineTukey(linePoints, point);
		vec.push_back(make_pair(point, distance));
	}
	sort(vec.begin(), vec.end(), CmpByValue());
	selectpointsInputOK.clear();
	for (int i = 0; i < vec.size() - countDelete; i++)
	{
		selectpointsInputOK.push_back(vec.at(i).first);
	}
	for (int i = vec.size() - countDelete; i < vec.size(); i++)
	{
		selectpointsInputNG.push_back(vec.at(i).first);
	}
	return 0;
}