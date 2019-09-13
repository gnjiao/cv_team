#pragma once


#include <boost\archive\text_iarchive.hpp>

#include <boost\archive\text_oarchive.hpp>

#include <boost\serialization\base_object.hpp>

#include <opencv2\core\core.hpp>

#include <vector>

#include "DataModel\ToolData.h"

#include "geo\Point2D.h"

#include "geo\Line2D.h"

#include "geo\Circle2D.h"

#include "geo\Rect2D.h"

#include "geo\RotRect2D.h"

#include "geo\AffineTrans.h"

/*
******这个类是输出结果，描述的是整个流程的每个算子的输出结果总和
******因为每个算子算出来的结果有很多种，都存放在这个类里面，以供后面的算子选用不同类型的结果
******
******
******
******
******
*/

using namespace std;

using namespace geo;

namespace toolModel

{

	class ToolDataMap

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & m_vectorInt;

			ar & m_vectorDouble;

			ar & m_vectorBool;

		}



	public:

		ToolDataMap();

		~ToolDataMap();



		ToolDataMap* Add(ToolData<int>* i, ToolDataType type = ToolDataType::eInt);

		ToolDataMap* Add(ToolData<double>* d, ToolDataType type = ToolDataType::eDouble);

		ToolDataMap* Add(ToolData<bool>* b, ToolDataType type = ToolDataType::eBool);

		ToolDataMap* Add(ToolData<std::string>* s, ToolDataType type = ToolDataType::eString);

		ToolDataMap* Add(ToolData<cv::Mat>* m, ToolDataType type = ToolDataType::eMat);

		ToolDataMap* Add(ToolData<Angle>* a, ToolDataType type = ToolDataType::eAngle);

		ToolDataMap* Add(ToolData<Point2D>* p, ToolDataType type = ToolDataType::ePoint);

		ToolDataMap* Add(ToolData<Line2D>* l, ToolDataType type = ToolDataType::eLine);

		ToolDataMap* Add(ToolData<Circle2D>* c, ToolDataType type = ToolDataType::eCircle);

		ToolDataMap* Add(ToolData<AffineTrans>* a, ToolDataType type = ToolDataType::eAffineTrans);



		std::vector<ToolData<int>*> vectorInt;

		std::vector<ToolData<double>*> vectorDouble;

		std::vector<ToolData<bool>*> vectorBool;

		std::vector<ToolData<std::string>*> vectorString;

		std::vector<ToolData<cv::Mat>*> vectorMat;

		std::vector<ToolData<Point2D>*> vectorPoint;

		std::vector<ToolData<Line2D>*> vectorLine;

		std::vector<ToolData<Circle2D>*> vectorCircle;

		std::vector<ToolData<double>*> vectorLength;

		std::vector<ToolData<double>*> vectorArea;

		std::vector<ToolData<Angle>*> vectorAngle;

		std::vector<ToolData<double>*> vectorScore;

		std::vector<ToolData<AffineTrans>*> vectorAffineTrans;

		std::vector<ToolData<cv::Mat>*> vectorRefMat;

		std::vector<ToolData<cv::Mat>*> vectorRegion;

		std::vector<ToolData<cv::Mat>*> vectorFeaturePoints;

	};

}
