#pragma once
#include "DataModel\BaseOperator.h"
#include "PreTreatment\Morphology.h"
#include "PreTreatment\MorphologyParam.h"

using namespace toolModel;

class MorphologyOperator : public BaseOperator
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseOperator>(*this);
		ar & input_mat;
		ar & m_mode;
		ar & m_dilateParam;
		ar & m_erodeParam;
		ar & m_advancedParam;
		ar & output_mat;
		ar & output_refMat;
	}
public:
	enum MorphologyMode
	{
		Dilate,
		Erode,
		Advanced,
	};
	MorphologyOperator();
	~MorphologyOperator();
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
	MorphologyMode m_mode = MorphologyMode::Dilate;
	DilateParam m_dilateParam;
	ErodeParam m_erodeParam;
	AdvancedMorphologyParam m_advancedParam;
private:
	Morphology* m_core = nullptr;
	NodeState _test(MatType type);
};