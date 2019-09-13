#pragma once
#include "DataModel\BaseOperator.h"
#include "FindCircle.h"
using namespace toolModel;
class FindCircleOperator : public toolModel::BaseOperator
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
		ar & m_resultType; 
		ar & m_gradThresh; 
		ar & m_fitType; 
		ar & m_maxIter; 
		ar & m_distThresh;
		ar & m_ItemCircle;
		ar & m_divideCounts;
		ar & m_divideWidth;
		ar & m_divideLength;
		ar & m_divideRects;
		ar & m_deleteCount;
		ar & output_circle;
		ar & output_center;
		ar & output_radius;
		ar & output_diameter;
		ar & output_circumference;
		ar & output_area;
	}
public:
	enum ErrorCode
	{
		Null,
		NG_Mat_Null,
		NG_Caliper_Count,
	};
	FindCircleOperator();
	~FindCircleOperator();

	void initOutput();
	void registerOutput();

	toolModel::NodeState execute();
	string getResultString();
	BaseNode* clone() override;
	void resetOutputValue() override;
	toolModel::NodeState test(cv::Mat src, vector<geo::RotRect2D> rects);
	//input
	toolModel::ToolData<cv::Mat>* input_mat = nullptr;
	toolModel::ToolData<cv::Mat>* input_refMat = nullptr;
	toolModel::ToolData<AffineTrans>* input_affineTrans = nullptr;
	//output
	toolModel::ToolData<geo::Circle2D>* output_circle = nullptr;
	toolModel::ToolData<geo::Point2D>* output_center = nullptr;
	toolModel::ToolData<double>* output_radius = nullptr;
	toolModel::ToolData<double>* output_diameter = nullptr;
	toolModel::ToolData<double>* output_circumference = nullptr;
	toolModel::ToolData<double>* output_area = nullptr;
	
	void setItemCircle(Circle2D c) { this->m_ItemCircle = c; }
	Circle2D getItemCircle() { return m_ItemCircle; }
	void setPolarProperty(int polarProperty) { m_polarProperty = polarProperty; setCoreParams(); }
	int getPolarProperty()const { return m_polarProperty; }
	void setResultType(int resultType) { m_resultType = resultType; setCoreParams(); }
	int getResultType()const { return m_resultType; }
	void setGradThresh(int gradThresh) { m_gradThresh = gradThresh; setCoreParams(); }
	int getGradThresh()const { return m_gradThresh; }
	void setFitType(int fitType) { m_fitType = fitType; setCoreParams(); }
	int getFitType()const { return m_fitType; }
	void setMaxIter(int maxIter) { m_maxIter = maxIter; setCoreParams(); }
	int getMaxIter()const { return m_maxIter; }
	void setDistThresh(int distThresh) { m_distThresh = distThresh; setCoreParams(); }
	int getDistThresh()const { return m_distThresh; }
	void setDeleteCount(int value) { m_deleteCount = value; }
	int getDeleteCount()const { return m_deleteCount; }

	void setDivideCounts(int counts) { m_divideCounts = counts; }
	int getDivideCounts() { return m_divideCounts; }
	void setCaliperWidth(int wid) { m_divideWidth = wid; }
	int getCaliperWidth() { return m_divideWidth; }
	void setCaliperLength(int leng) { m_divideLength = leng; }
	int getCaliperLength() { return m_divideLength; }
	vector<geo::RotRect2D> getDivideRects()const { return m_divideRects; }
	void setDivideRects(vector<geo::RotRect2D> Rects) { m_divideRects = Rects; }
	vector<geo::Point2D> getPointsOK();
	vector<geo::Point2D> getPointsNG();
	void setCoreParams()
	{
		m_caliper->SetPolarProperty(m_polarProperty);
		m_caliper->SetResultType(m_resultType);
		m_caliper->SetThresholdValue(m_gradThresh);
		m_findcircle->SetDistThresh(m_distThresh);
		m_findcircle->SetRansacEnable(m_fitType);
		m_findcircle->SetmaxIter(m_maxIter);
	}
	FindCircleOperator::ErrorCode m_errorCode;
private:
	Calipers * m_caliper = nullptr;
	geo::Circle2D m_ItemCircle= geo::Circle2D(Point2D(100, 100), 50);
	int m_polarProperty = 0;
	int m_resultType = 1;
	int m_gradThresh = 30;
	FindCircle * m_findcircle = nullptr;
	int m_fitType = 1;
	int m_maxIter = 2000;
	int m_distThresh = 5;
	int m_deleteCount = 0;
	int m_divideCounts = 10;
	int m_divideWidth = 10;
	int m_divideLength = 20;
	vector<geo::RotRect2D> m_divideRects;
	vector<cv::Point2d> m_pointsOK;
	vector<cv::Point2d> m_pointsNG;
	cv::Point3f m_circle_para;
	toolModel::NodeState _test(cv::Mat src, vector<geo::RotRect2D> rects);
};