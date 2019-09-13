#pragma once
#include "DataModel\BaseOperator.h"
#include "Inspection\EntityManager.h"

class AcquireImageOperator : public BaseOperator
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseOperator>(*this);
		ar & output_mat;
		ar & output_refMat;
		ar & output_nullAffineTrans;
	}
public:
	AcquireImageOperator();
	~AcquireImageOperator();
	void initOutput();
	void registerOutput();
	NodeState execute();
	string getResultString();
	BaseNode* clone() override;
	void resetOutputValue() override;
	void setCoreParams();
	ToolData<cv::Mat>* output_mat = nullptr;
	ToolData<cv::Mat>* output_refMat = nullptr;
	ToolData<AffineTrans>* output_nullAffineTrans = nullptr;
};
