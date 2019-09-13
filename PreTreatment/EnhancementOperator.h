#pragma once
#include "DataModel\BaseOperator.h"
#include "PreTreatment\Enhancement.h"
using namespace toolModel;
class EnhancementOperator : public BaseOperator
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseOperator>(*this);
		ar & input_mat;
		ar & m_mode;
		ar & output_mat;
		ar & output_refMat;
	}
public:
	enum EnhancementMode
	{
		EqualizeHist,
		Laplace,
		Gamma,
		Log
	};
	EnhancementOperator();
	~EnhancementOperator();
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
	EnhancementMode m_mode = EnhancementMode::EqualizeHist;
private:
	Enhancement* m_core = nullptr;
	NodeState _test(MatType type);
};