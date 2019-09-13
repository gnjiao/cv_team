#pragma once
#include "DataModel\BaseOperator.h"
#include "Blob.h"
#include <boost/serialization/version.hpp>
using namespace toolModel;
class BlobOperator : public BaseOperator
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseOperator>(*this);
		ar & input_mat;
		ar & m_params;
		ar & output_point;
		ar & output_area;
		if (version>0)
			ar& output_affineTrans;
		ar & output_region;
	}
public:
	BlobOperator();
	~BlobOperator();
	SIMPLEBLOB_SPACE::SimpleBlob *pblob;
	void initOutput();
	void registerOutput();
	NodeState execute();
	string getResultString();
	BaseNode* clone() override;
	void resetOutputValue() override;
	void drawFeatures(cv::Mat mat) override;
	NodeState test(cv::Mat src);
	//input
	toolModel::ToolData<cv::Mat>* input_mat = nullptr;
	toolModel::ToolData<cv::Mat>* input_refMat = nullptr;
	//output
	toolModel::ToolData<Point2D>* output_point = nullptr;
	toolModel::ToolData<double>* output_area = nullptr;
	ToolData<AffineTrans>* output_affineTrans = nullptr;
	ToolData<cv::Mat>* output_region = nullptr;

	void setParams(SIMPLEBLOB_SPACE::Params params);
	void setCoreParams();
	SIMPLEBLOB_SPACE::Params getParameters() const;

	void setROI(Rect2D rect);
	Rect2D getROI();

private:
	SIMPLEBLOB_SPACE::Params m_params;
	NodeState _test(cv::Mat src);
	bool ROI_Checked = false;
	Rect2D ROI = Rect2D(Point2D(200, 200), 100, 100);

};
BOOST_CLASS_VERSION(BlobOperator, 1)