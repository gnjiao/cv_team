#pragma once
#include "DataModel\BaseOperator.h"
#include "Recognition\Barcode.h"
using namespace toolModel;
class BarcodeOperator : public BaseOperator
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseOperator>(*this);
		ar & input_mat;
		ar & output_string;
	}
public:
	BarcodeOperator();
	~BarcodeOperator();
	void initOutput();
	void registerOutput();
	NodeState execute();
	string getResultString();
	BaseNode* clone() override;
	void resetOutputValue() override;
	NodeState test(MatType type);
	void setCoreParams();
	ToolData<cv::Mat>* input_mat = nullptr;
	ToolData<string>* output_string = nullptr;
	ToolData<cv::Mat>*output_refMat = nullptr;
	ToolData<cv::Mat>* input_refMat = nullptr;
private:
	Barcode* m_core = nullptr;
	NodeState _test(MatType type);
};