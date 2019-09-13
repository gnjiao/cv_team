#pragma once
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>
#include <boost\serialization\base_object.hpp>
#include <boost\serialization\vector.hpp>
#include <opencv2\core\core.hpp>
#include <vector>
#include "geo\Rect2D.h"


struct CreateTemplateParam

{

private:

	friend class boost::serialization::access;

	template<class Archive>

	void serialize(Archive & ar, const unsigned int version)

	{

		ar & isCreated;

		ar & minThreshold;

		ar & maxThreshold;

		ar & roi;

		ar & center;

		ar & featurePoints;

	}



public:

	cv::Mat srcMat;

	bool isCreated = false;

	int minThreshold = 10;

	int maxThreshold = 30;

	geo::Rect2D roi = Rect2D(Point2D(0, 0), Point2D(100, 100));

	geo::Point2D center;

	std::vector<geo::Point2D> featurePoints;



	void reset()

	{

		isCreated = false;

		center = geo::Point2D();

		featurePoints.clear();

	}

};



struct FindTemplateParam

{

private:

	friend class boost::serialization::access;

	template<class Archive>

	void serialize(Archive & ar, const unsigned int version)

	{

		ar & minScore;

		ar & maxMatchNum;

		ar & minAngle;

		ar & maxAngle;

		ar & minScale;

		ar & maxScale;

		ar & greediness;

	}



public:

	double minScore = 0.7;

	int maxMatchNum = 1;

	double minAngle = -180;

	double maxAngle = 180;

	double minScale = 1;

	double maxScale = 1;

	double greediness = 0.7;

};



struct TemplateResult

{

private:

	friend class boost::serialization::access;

	template<class Archive>

	void serialize(Archive & ar, const unsigned int version)

	{

		ar & score;

		ar & rotation;

		ar & scale;

		ar & center;

		ar & translation;

		ar & timeSpan;

		ar & featurePoints;

	}



public:

	double score = 0;

	Angle rotation;

	double scale = 1;

	Point2D center;

	Point2D translation;

	double timeSpan = 0;

	vector<Point2D> featurePoints;

};






