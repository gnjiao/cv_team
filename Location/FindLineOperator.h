#pragma once
#include "DataModel\BaseOperator.h"
#include "FindLine.h"
using namespace geo;
using namespace toolModel;
class FindLineOperator : public toolModel::BaseOperator
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseOperator>(*this);
		ar & input_mat;
		ar & input_affineTrans;
		ar & m_polarProperty;
		ar & m_gradThresh;
		ar & m_filterType;
		ar & m_resultType;
		ar & m_maxIter;
		ar & m_deleteCount;
		ar & m_distThresh;
		ar & m_divideCounts;
		ar & m_divideWidth;
		ar & m_aimRect;
		ar & m_divideRects;
		ar & output_midPoint;
		ar & output_startPoint;
		ar & output_endPoint;
		ar & output_line;
		ar & output_length;
	}
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	enum ErrorCode
	{
		Null,
		NG_Mat_Null,
		NG_Caliper_Count,
		NG_Filter,
		NG_Filter_Count
	};
	FindLineOperator();
	~FindLineOperator();
	//
	BaseNode* clone() override;
	void resetOutputValue() override;
	//
	void initOutput();
	void registerOutput();
	//
	toolModel::NodeState execute();
	toolModel::NodeState test(cv::Mat src, vector<RotRect2D> caliperRects, RotRect2D &aimRect);
	//input
	toolModel::ToolData<cv::Mat>* input_mat = nullptr;
	toolModel::ToolData<cv::Mat>* input_refMat = nullptr;
	toolModel::ToolData<AffineTrans>* input_affineTrans = nullptr;
	//output
	toolModel::ToolData<Point2D>* output_midPoint = nullptr;
	toolModel::ToolData<Point2D>* output_startPoint = nullptr;
	toolModel::ToolData<Point2D>* output_endPoint = nullptr;
	toolModel::ToolData<Line2D>* output_line = nullptr;
	toolModel::ToolData<double>* output_length = nullptr;
	//get output
	Line2D getResultLine() { return output_line->GetValue(); }
	vector<Point2D> getPointsOK();
	vector<Point2D> getPointsNG();
	void setPointsOK(vector<geo::Point2D> ptsOK);
	void setPointNG(vector<geo::Point2D> ptsNG);
	string getResultString();
	//S&G:core param
	void setPolarProperty(int polarProperty) { m_polarProperty = polarProperty; setCoreParams(); }
	int getPolarProperty()const { return m_polarProperty; }
	void setResultType(int resultType) { m_resultType = resultType; setCoreParams(); }
	int getResultType()const { return m_resultType; }
	void setGradThresh(int gradThresh) { m_gradThresh = gradThresh; setCoreParams(); }
	int getGradThresh()const { return m_gradThresh; }
	void setFilterType(int fitType) { m_filterType = fitType; setCoreParams(); }
	int getFilterType()const { return m_filterType; }
	void setMaxIter(int maxIter) { m_maxIter = maxIter; setCoreParams(); }
	int getMaxIter()const { return m_maxIter; }
	void setDistThresh(int distThresh) { m_distThresh = distThresh; setCoreParams(); }
	int getDistThresh()const { return m_distThresh; }
	void setDeleteCount(int value) { m_deleteCount = value; }
	int getDeleteCount()const { return m_deleteCount; }
	//S&G:
	void setDivideCounts(int counts) { m_divideCounts = counts; }
	int getDivideCounts() { return m_divideCounts; }
	void setCaliperWidth(int wid) { m_divideWidth = wid; }
	int getCaliperWidth() { return m_divideWidth; }
	void setAimRect(RotRect2D &rect) { m_aimRect = rect; }
	RotRect2D getAimRect() { return m_aimRect; }
	void setDivideRects(vector<RotRect2D> Rects) { m_divideRects = Rects; }
	vector<RotRect2D> getDivideRects()const { return m_divideRects; }
	void setCoreParams();
	FindLineOperator::ErrorCode m_errorCode;
	//FindLineOperator::ErrorCode getErrorCode() { return this->m_errorCode; }
private:
	toolModel::NodeState _test(cv::Mat src, vector<RotRect2D> &caliperRects, RotRect2D &aimRect);
	Calipers * m_caliper = nullptr;
	int m_polarProperty = 0;
	int m_gradThresh = 30;
	FindLine * m_findline = nullptr;
	int m_filterType = 0;
	int m_resultType = 1;
	int m_maxIter = 2000;
	int m_distThresh = 5;
	int m_deleteCount = 0;
	int m_divideCounts = 5;
	int m_divideWidth = 10;
	RotRect2D m_aimRect;
	vector<RotRect2D> m_divideRects;
	vector<cv::Point2d> m_pointsOK;
	vector<cv::Point2d> m_pointsNG;
};
