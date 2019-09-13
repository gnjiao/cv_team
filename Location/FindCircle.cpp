#include "FindCircle.h"

FindCircle::FindCircle()
{
	m_distThresh = 20;
	m_maxIter = 2000;
	m_ransacEnable = true;
}

FindCircle::~FindCircle()
{
}

int FindCircle::FitCircle(vector<cv::Point2d> pointsInput, Point3f & circlePara, vector<cv::Point2d> &pointsOutputOK, vector<cv::Point2d> &pointsOutputNG, int countDelete)
{
	pointsOutputOK.clear();
	pointsOutputNG.clear();
	if (m_ransacEnable == true)
	{
		FilterRansac(pointsInput, circlePara, pointsOutputOK, pointsOutputNG, countDelete);
	}
	else
	{
		FitPoints(pointsInput, circlePara, pointsOutputOK, pointsOutputNG, countDelete);
	}
	return 0;
}
int FindCircle::FilterRansac(vector<cv::Point2d> pointsInput, Point3f & circlePara, vector<cv::Point2d> &pointsOutputOK, vector<cv::Point2d> &pointsOutputNG, int countDelete)
{
	if (pointsInput.size() < 3)
	{
		return -1;
	}
	if (m_maxIter <= 0)
	{
		return -1;
	}
	if (m_distThresh <= 0)
	{
		return -1;
	}
	vector<cv::Point2d> selectpointsInputOK;
	vector<cv::Point2d> selectpointsInputNG;
	int k = m_maxIter;
	vector<cv::Point2d> tmpSelectpointsInputOK;
	vector<cv::Point2d> tmpSelectpointsInputNG;
	vector<Point2d> chosedPoint;
	double r = 0;
	Point2d center;
	double dist = 0;
	cv::RNG rng((uint64)-1);
	for (int i = 0; i < m_maxIter; i++)
	{
		getThreePoints(pointsInput, chosedPoint, rng);

		FitCircleBy3Point(pointsInput, chosedPoint, r, center);
		for (int j = 0; j < pointsInput.size(); j++)
		{
			dist = abs(r - sqrt(pow(pointsInput[j].x - center.x, 2) + pow(pointsInput[j].y - center.y, 2)));
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
			k = log(0.01) / log(1 - pow((double)tmpSelectpointsInputOK.size() / pointsInput.size(), 3));
			selectpointsInputOK.clear();
			selectpointsInputNG.clear();
			selectpointsInputOK.assign(tmpSelectpointsInputOK.begin(), tmpSelectpointsInputOK.end());
			selectpointsInputNG.assign(tmpSelectpointsInputNG.begin(), tmpSelectpointsInputNG.end());
		}
		tmpSelectpointsInputOK.clear();
		tmpSelectpointsInputNG.clear();
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
	FitPoints(pointsOutputOK, circlePara, pointsOutputOK, pointsOutputNG);
	return 0;
}

int FindCircle::FitPoints(vector<cv::Point2d> pointsInput, Point3f & circlePara, vector<cv::Point2d> &pointsOutputOK, vector<cv::Point2d> &pointsOutputNG, int countDelete)
{
	if (pointsInput.size()<3)
	{
		return -1;
	}
	float x1 = 0;
	float x2 = 0;
	float x3 = 0;
	float y1 = 0;
	float y2 = 0;
	float y3 = 0;
	float x1y1 = 0;
	float x1y2 = 0;
	float x2y1 = 0;
	int num;
	vector<Point2d>::iterator k;

	for (k = pointsInput.begin(); k != pointsInput.end(); k++)
	{
		x1 = x1 + (*k).x;
		x2 = x2 + (*k).x * (*k).x;
		x3 = x3 + (*k).x * (*k).x * (*k).x;
		y1 = y1 + (*k).y;
		y2 = y2 + (*k).y * (*k).y;
		y3 = y3 + (*k).y * (*k).y * (*k).y;
		x1y1 = x1y1 + (*k).x * (*k).y;
		x1y2 = x1y2 + (*k).x * (*k).y * (*k).y;
		x2y1 = x2y1 + (*k).x * (*k).x * (*k).y;
	}
	float C, D, E, G, H, a, b, c;
	num = pointsInput.size();
	C = num*x2 - x1*x1;
	D = num*x1y1 - x1*y1;
	E = num*x3 + num*x1y2 - x1*(x2 + y2);
	G = num*y2 - y1*y1;
	H = num*x2y1 + num*y3 - y1*(x2 + y2);
	a = (H*D - E*G) / (C*G - D*D);
	b = (H*C - E*D) / (D*D - G*C);
	c = -(x2 + y2 + a*x1 + b*y1) / num;
	circlePara.x = -a / 2; 
	circlePara.y = -b / 2; 
	circlePara.z = sqrt(a*a + b*b - 4 * c) / 2;
	pointsOutputOK.clear();
	for (int i = 0; i<pointsInput.size(); i++)
	{
		pointsOutputOK.push_back(pointsInput[i]);
	}
	if (pointsOutputNG.size()<countDelete)
	{
		int tempNum = countDelete - pointsOutputNG.size();
		DeletePoints(pointsOutputOK, pointsOutputNG, tempNum);
	}
	return 0;
}

int FindCircle::getThreePoints(vector<cv::Point2d> pointsInput, vector<Point2d> &chosedPoint, cv::RNG& rng)
{
	while (true)
	{
		chosedPoint.clear();
		chosedPoint.push_back(pointsInput[rng.uniform(0, pointsInput.size())]);
		chosedPoint.push_back(pointsInput[rng.uniform(0, pointsInput.size())]);
		chosedPoint.push_back(pointsInput[rng.uniform(0, pointsInput.size())]);
		if ((chosedPoint[0] != chosedPoint[1]) &&
			(chosedPoint[0] != chosedPoint[2]) &&
			(chosedPoint[1] != chosedPoint[2]))
		{
			break;
		}
	}
	return 0;
}

int FindCircle::FitCircleBy3Point(vector<cv::Point2d> pointsInput, vector<Point2d> chosedPoint, double &r, Point2d &center)
{
	Point2d p1, p2, p3;
	p1 = chosedPoint[0];
	p2 = chosedPoint[1];
	p3 = chosedPoint[2];
	double xMove = p1.x;
	double yMove = p1.y;
	p1.x = 0;
	p1.y = 0;
	p2.x = p2.x - xMove;
	p2.y = p2.y - yMove;
	p3.x = p3.x - xMove;
	p3.y = p3.y - yMove;
	double x1 = p2.x,y1 = p2.y,x2 = p3.x,y2 = p3.y;
	double m = 2.0 * (x1 * y2 - y1 * x2);
	double x0 = (x1 * x1 * y2 - x2 * x2 * y1 + y1 * y2 * (y1 - y2)) / m;
	double y0 = (x1 * x2 * (x2 - x1) - y1 * y1 * x2 + x1 * y2 * y2) / m;
	r = sqrt(x0 * x0 + y0 * y0);
	center.x = x0 + xMove;
	center.y = y0 + yMove;
	return 0;
}
int FindCircle::DeletePoints(vector<Point2d>& selectpointsInputOK, vector<Point2d>& selectpointsInputNG, int countDelete)
{
	Point3f circlePoints;
	vector<pair<Point2d, double>> vec;
	double distance = 0;
	FindCircle::FitPoints(selectpointsInputOK, circlePoints, selectpointsInputOK, selectpointsInputNG);
	for each (auto point in selectpointsInputOK)
	{
		distance = abs(circlePoints.z - sqrt(pow(point.x - circlePoints.x, 2) + pow(point.y - circlePoints.y, 2)));
		vec.push_back(make_pair(point, distance));
	}
	sort(vec.begin(), vec.end(), CmpByValue2());
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
