#pragma once
#include "DataModel\BaseOperator.h"
#include "ShapeMatchAlgorithm.h"
#include "ShapeMatchParam.h"

using namespace toolModel;
class ShapeMatchOperator : public BaseOperator
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseOperator>(*this);
		ar & input_mat;
		ar & m_findTemplateParam;
		ar & output_best_affineTrans;
		ar & output_best_center;
		ar & output_best_translation;
		ar & output_best_rotation;
		ar & output_best_score;
		ar & output_Feature_Points;
		ar & ROI;
	}
public:
	enum ErrorCode
	{
		Null,
		NG_Mat_Null,
		NG_SetMatchPara,
		NG_FindTemplate,
		NG_Not_Find
	};
	ShapeMatchOperator();
	~ShapeMatchOperator();
	void initOutput();
	void registerOutput();
	NodeState execute();
	string getResultString();
	BaseNode* clone() override;
	void resetOutputValue() override;
	void drawFeatures(cv::Mat)override;
	NodeState test(cv::Mat src);
	void setCoreParams();
	ShapeMatchOperator::ErrorCode m_errorCode;
	//input
	ToolData<cv::Mat>* input_mat = nullptr;
	ToolData<cv::Mat>* input_refMat = nullptr;
	//output
	ToolData<AffineTrans>* output_best_affineTrans = nullptr;
	ToolData<Point2D>* output_best_center = nullptr;
	ToolData<Point2D>* output_best_translation = nullptr;
	ToolData<Angle>* output_best_rotation = nullptr;
	ToolData<double>* output_best_score = nullptr;
	ToolData<cv::Mat> * output_Feature_Points = nullptr;
	vector<TemplateResult> output_allResults;
	TemplateResult output_bestResult;
	CreateTemplateParam m_createTemplateParam;
	string getTemplateImageFileName()const;
	string getTemplateFileName()const;
	int getFeaturePoints(const cv::Mat src, Rect2D roi, const double minThresh, const double maxThresh, vector<cv::Point> &featurePoints);
	int createTemplate(const cv::Mat src, Rect2D roi, const vector<cv::Point> featurePoints, int minThreshold, int maxThreshold);
	int createTemplate();
	int setFindTemplateParams(const double minMatchRate, const double greediness, const double startAngle, const double endAngle,
		const double minScale, const double maxScale, const int maxMatchNum);
	int findTemplate(const cv::Mat src, vector<double>& matchScore, vector<double>& matchAngle, vector<double>& matchScale,
		vector<cv::Point>& matchPoint, vector <vector<cv::Point>>& resultFeaturePoints, double& timeSpan);
	int saveTemplate();
	int loadTemplate();
	void deleteTemplate();
	bool ROI_Checked = false;
	void setROI(Rect2D rect);
	Rect2D getROI();

	FindTemplateParam m_findTemplateParam;
private:	
	CShapeMatchAlgorithm* m_core = nullptr;
	NodeState _test(cv::Mat src);
	Rect2D ROI = Rect2D(Point2D(200, 200), 100, 100);
};