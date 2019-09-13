#pragma once
#include "DataModel\BaseOperator.h"
#include "PreTreatment\ThresholdParam.h"
#include "PreTreatment\Threshold.h"

using namespace toolModel;

class ThresholdOperator : public BaseOperator
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseOperator>(*this);
		ar & input_mat;
		ar & m_mode;
		ar & m_fixParam;
		ar & m_autoParam;
		ar & m_segmentParam;
		ar & m_twoPeakParam;
		ar & output_mat;
		ar & output_refMat;
	}
public:
	enum ThresholdMode
	{
		Fix,
		Auto,
		Segment,
		TwoPeak
	};
	ThresholdOperator();
	~ThresholdOperator();
	void initOutput();
	void registerOutput();
	NodeState execute();
	string getResultString();
	BaseNode* clone() override;
	void resetOutputValue() override;
	NodeState test(MatType type);
	void setCoreParams();
	ToolData<cv::Mat>* input_mat = nullptr;
	ToolData<cv::Mat>* input_refMat = nullptr;
	ToolData<cv::Mat>* output_mat = nullptr;
	ToolData<cv::Mat>* output_refMat = nullptr;
	ThresholdOperator::ThresholdMode m_mode = ThresholdMode::Fix;
	ThresholdFixParam m_fixParam;
	ThresholdAutoParam m_autoParam;
	ThresholdSegmentParam m_segmentParam;
	ThresholdTwoPeakParam m_twoPeakParam;
private:
	Threshold* m_threshold = nullptr;
	NodeState _test(MatType type);
};