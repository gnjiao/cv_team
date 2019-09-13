#pragma once
#include <boost/serialization/version.hpp>
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include <iterator>
#include <limits>
#include "opencv2/core/matx.hpp"
#include "opencv2/core/types.hpp"
#include <string>

namespace SIMPLEBLOB_SPACE
{
	struct Params;
}
BOOST_CLASS_VERSION(SIMPLEBLOB_SPACE::Params, 3)
namespace SIMPLEBLOB_SPACE
{
	enum Mode
	{
		INSIDE,
		OUTSIDE,
	};
	struct feature
	{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & mode;
			ar & minValue;
			ar & maxValue;
			ar & inuse;
			ar & name;
		}
	public:
		feature();
		Mode mode;
		double minValue;
		double maxValue;
		bool inuse;
		std::string name;
	};
	struct Params
	{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & filterByColor;
			ar & blobColor;
			ar & minArea;
			ar & maxArea;
			ar & threshold;
			if (version > 0)
			{
				ar & Area;
				ar & Circularity;
				ar & Inertia;
				ar & Convexity;
				ar & Rectangularity;
			}
			if (version > 1)
			{
				ar & BolbWide;
				ar & BlobHeight;
				ar & Compactness;
				ar & Conlength;
			}
			if (version > 2)
			{
				ar&center_x;
				ar&center_y;
			}
		}
	public:
		Params();
		float thresholdStep;
		int threshold;
		float maxThreshold;
		size_t minRepeatability;
		float minDistBetweenBlobs;
		bool filterByColor;
		uchar blobColor;
		bool filterByArea;
		float minArea, maxArea;
		bool filterByCircularity;
		float minCircularity, maxCircularity;
		bool filterByInertia;
		float minInertiaRatio, maxInertiaRatio;
		bool filterByConvexity;
		float minConvexity, maxConvexity;
		feature Area, Circularity, Inertia, Convexity, Rectangularity, BolbWide, BlobHeight, Compactness, Conlength;
		bool museroi;
		int mroi_LeftTopX;
		int mroi_LeftTopY;
		int mwidth;
		int mheight;
		double center_x, center_y;
	};
	struct CResult
	{
		cv::Point2d location;
		double radius = 0.0f;
		std::vector<cv::Point>m_contours;
		double area = 0.0f;
		double Circularity = 0;
		double Inertia = 0;
		double Convexity = 0;
		double Rectangularity = 0;
		double wide = 0;
		double heiht = 0;
		double Compactness = 0;
		double Conlength = 0;
	};
	class SimpleBlob
	{
	public:
		SimpleBlob(const Params &parameters);
		SimpleBlob();
		void findBlobs(cv::InputArray Image);
		void findBlobs(cv::InputArray Image, cv::Rect roi);
		void Draw_results(cv::Mat &picture, int index = -1);
		SimpleBlob *set_params(Params);
		Params get_params(void) const;
	private:
		Params m_params;
		cv::Rect mroi;
		bool useroi;
		bool checkroi(cv::Rect roi, cv::Mat &m);
		bool static compare_are(CResult A, CResult B);
		uchar PointInPloygonTest(std::vector<cv::Point2i> contour, cv::Mat Image);
		bool InFeature(feature &test, double &ration);
	public:
		std::vector<CResult> m_result;
	};
}