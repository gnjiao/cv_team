#pragma once
#include "DataModel\BaseOperator.h"
#include "PreTreatment\Smooth.h"
#include "PreTreatment\SmoothParam.h"

using namespace toolModel;

class SmoothOperator : public BaseOperator
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseOperator>(*this);
		ar & input_mat;
		ar & m_mode;
		ar & m_blurSmoothParam;
		ar & m_gaussianSmoothParam;
		ar & m_boxSmoothParam;
		ar & m_bilateralSmoothParam;
		ar & m_medianSmoothParam;
		ar & output_mat;
		ar & output_refMat;
	}
public:
	enum SmoothMode
	{
		Blur,
		Gaussian,
		Box,
		Bilateral,
		Median
	};
	SmoothOperator();
	~SmoothOperator();
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
	SmoothMode m_mode = SmoothMode::Blur;
	BlurSmoothParam m_blurSmoothParam;
	GaussianSmoothParam m_gaussianSmoothParam;
	BoxSmoothParam m_boxSmoothParam;
	BilateralSmoothParam m_bilateralSmoothParam;
	MedianSmoothParam m_medianSmoothParam;
private:
	Smooth* m_core = nullptr;
	NodeState _test(MatType type);
};