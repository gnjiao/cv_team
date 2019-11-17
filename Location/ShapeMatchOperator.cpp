#include "ShapeMatchOperator.h"
#include <direct.h>

ShapeMatchOperator::ShapeMatchOperator()
{
	m_nodeName = "shapeMatch";
	m_showText = "shapeMatch";
	m_core = new CShapeMatchAlgorithm();
}
ShapeMatchOperator::~ShapeMatchOperator()
{
	delete m_core;
}
void ShapeMatchOperator::initOutput()
{
	output_best_affineTrans = createOutput<AffineTrans>(ToolDataType::eAffineTrans);
	output_best_center = createOutput<Point2D>(ToolDataType::ePoint)->SetDisplayEnabled(true);
	output_best_translation = createOutput<Point2D>(ToolDataType::ePoint);
	output_best_rotation = createOutput<Angle>(ToolDataType::eAngle);
	output_best_score = createOutput<double>(ToolDataType::eDouble);
	output_Feature_Points = createOutput<cv::Mat>(ToolDataType::eFeaturePoints);
	output_Feature_Points->SetDisplayEnabled(true);
	//output_best_affineTrans->SetShowText(TOOLBOXMGR->shapeMatch.map[TOOLBOXMGR->shapeMatch.affineTrans]);
	//output_best_center->SetShowText(TOOLBOXMGR->shapeMatch.map[TOOLBOXMGR->shapeMatch.center]);
	//output_best_translation->SetShowText(TOOLBOXMGR->shapeMatch.map[TOOLBOXMGR->shapeMatch.translation]);
	//output_best_rotation->SetShowText(TOOLBOXMGR->shapeMatch.map[TOOLBOXMGR->shapeMatch.rotation]);
	//output_best_score->SetShowText(TOOLBOXMGR->shapeMatch.map[TOOLBOXMGR->shapeMatch.score]);
	//output_Feature_Points->SetShowText(TOOLBOXMGR->shapeMatch.map[TOOLBOXMGR->shapeMatch.featurepoints]);
}
void ShapeMatchOperator::registerOutput()
{
	output()->Add(output_best_affineTrans, ToolDataType::eAffineTrans);
	output()->Add(output_best_center, ToolDataType::ePoint);
	output()->Add(output_best_translation, ToolDataType::ePoint);
	output()->Add(output_best_rotation, ToolDataType::eAngle);
	output()->Add(output_best_score, ToolDataType::eDouble);
	output()->Add(output_Feature_Points, ToolDataType::eFeaturePoints);
	output_best_affineTrans->SetShowText("shapeMatch-affineTrans");
	output_best_center->SetShowText("shapeMatch-center");
	output_best_translation->SetShowText("shapeMatch-translation");
	output_best_rotation->SetShowText("shapeMatch-rotation");
	output_best_score->SetShowText("shapeMatch-score");
	output_Feature_Points->SetShowText("shapeMatch-featurepoints");
}
NodeState ShapeMatchOperator::execute()
{
	if (input_mat == nullptr)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	return test(this->input_mat->GetValue());
}
string ShapeMatchOperator::getResultString()
{
	string str;
	if (output_best_affineTrans->GetOutputEnabled() && output_best_affineTrans->GetOutputChecked())
	{

	}
	if (output_best_center->GetOutputEnabled() && output_best_center->GetOutputChecked())
	{
		str += transPoint2D(output_best_center->GetValue()).ToString();
	}
	if (output_best_translation->GetOutputEnabled() && output_best_translation->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += transPoint2D(output_best_translation->GetValue()).ToString();
	}
	if (output_best_rotation->GetOutputEnabled() && output_best_rotation->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += output_best_rotation->GetValue().ToDegString();
	}
	if (output_best_score->GetOutputEnabled() && output_best_score->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += to_string(output_best_score->GetValue());
	}
	return str;
}
BaseNode * ShapeMatchOperator::clone()
{
	ShapeMatchOperator *newnode = new ShapeMatchOperator();
	newnode->m_findTemplateParam = this->m_findTemplateParam;
	newnode->input_mat = createClone<cv::Mat>(this->input_mat);
	newnode->input_refMat = createClone<cv::Mat>(this->input_refMat);
	newnode->initOutput();
	newnode->setCoreParams();
	return newnode;
}
void ShapeMatchOperator::resetOutputValue()
{
	if (nullptr != output_best_affineTrans)
		output_best_affineTrans->SetValue(AffineTrans());
	if (nullptr != output_best_center)
		output_best_center->SetValue(Point2D());
	if (nullptr != output_best_translation)
		output_best_translation->SetValue(Point2D());
	if (nullptr != output_best_rotation)
		output_best_rotation->SetValue(Angle());
	if (nullptr != output_best_score)
		output_best_score->SetValue(0);
}
void ShapeMatchOperator::drawFeatures(cv::Mat imag)
{
	auto it = output_bestResult.featurePoints;
	for (size_t i = 0; i < it.size(); i++)
	{
		cv::drawMarker(imag, cv::Point2i(it.at(i).X(), it.at(i).Y()), cv::Scalar(255, 255, 0), cv::MarkerTypes::MARKER_DIAMOND, 3);
	}
}
NodeState ShapeMatchOperator::test(cv::Mat src)
{
	cv::Mat srcImage;
	if (src.channels() > 1)
	{
		cv::cvtColor(src, srcImage, CV_BGR2GRAY);
	}
	else
	{
		src.copyTo(srcImage);
	}
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	m_state = _test(srcImage);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	setDuration(duration_cast<chrono::microseconds>(t2 - t1).count() / 1000.0);
	return m_state;
}
NodeState ShapeMatchOperator::_test(cv::Mat src)
{
	output_bestResult.featurePoints.clear();
	if (!src.data)
	{
		m_errorCode = ErrorCode::NG_Mat_Null;
		return NodeState::Failure;
	}
	cv::Rect roi;
	if (this->ROI_Checked)
	{
		roi = cv::Rect(cv::Point(this->ROI.LeftTop().X(), this->ROI.LeftTop().Y()),
			cv::Point(this->ROI.RightBot().X(), this->ROI.RightBot().Y()));
	}
	else
	{
		roi = cv::Rect(cv::Point(0, 0), cv::Point(src.cols, src.rows));
	}
	if (0 != m_core->SetMatchPara(m_findTemplateParam.minScore, m_findTemplateParam.greediness,
		m_findTemplateParam.minAngle, m_findTemplateParam.maxAngle, m_findTemplateParam.minScale,
		m_findTemplateParam.maxScale, m_findTemplateParam.maxMatchNum, roi))
	{
		m_errorCode = ErrorCode::NG_SetMatchPara;
		return NodeState::Failure;
	}
	vector<double> matchScores;
	vector<double> matchAngles;
	vector<double> matchScales;
	vector<cv::Point> matchPoints;
	vector<vector<cv::Point>> featurePoints;
	double timeSpan;
	if (0 != this->findTemplate(src, matchScores, matchAngles, matchScales, matchPoints, featurePoints, timeSpan))
	{
		m_errorCode = ErrorCode::NG_FindTemplate;;
		return NodeState::Failure;
	}
	if (matchScores.size() < 1)
	{
		m_errorCode = ErrorCode::NG_Not_Find;
		return NodeState::Failure;
	}
	this->output_allResults.clear();
	for (int i = 0; i < matchScores.size(); i++)
	{
		TemplateResult result;
		result.score = matchScores[i];
		result.rotation = Angle(matchAngles[i], geo::RadOrDeg::Deg);
		result.scale = matchScales[i];
		//swap x and y
		result.center = Point2D(matchPoints[i].y, matchPoints[i].x);
		result.translation = result.center - m_createTemplateParam.center;
		for each (auto var in featurePoints.at(i))
		{
			result.featurePoints.push_back(Point2D(var.x, var.y));
		}
		result.timeSpan = timeSpan;
		this->output_allResults.push_back(result);
	}
	//best reuslt
	output_bestResult = this->output_allResults.at(0);
	output_best_score->SetValue(output_bestResult.score);
	output_best_center->SetValue(output_bestResult.center);
	output_best_translation->SetValue(output_bestResult.translation);
	output_best_rotation->SetValue(output_bestResult.rotation);
	AffineTrans affineTrans;
	affineTrans.Center = output_bestResult.center;
	affineTrans.Translation = output_bestResult.translation;
	affineTrans.Rotation = output_bestResult.rotation;
	output_best_affineTrans->SetValue(affineTrans);
	m_errorCode = ErrorCode::Null;
	return NodeState::Success;
}
string ShapeMatchOperator::getTemplateImageFileName() const
{
	return this->nodeDirectory() + "\\template.jpg";
}
string ShapeMatchOperator::getTemplateFileName() const
{
	return this->nodeDirectory() + "\\" + this->GetNodeName() + to_string(this->GetNodeKey()) + ".template";
}
int ShapeMatchOperator::getFeaturePoints(const cv::Mat src, Rect2D roi, const double minThresh, const double maxThresh, vector<cv::Point>& featurePoints)
{
	cv::Rect cr = cv::Rect(cv::Point(roi.LeftTop().X(), roi.LeftTop().Y()), cv::Point(roi.RightBot().X(), roi.RightBot().Y()));
	int ret = m_core->GetFeaturePoints(src, cr, minThresh, maxThresh, featurePoints);
	if (ret != 0)return ret;
	return 0;
}
int ShapeMatchOperator::createTemplate(const cv::Mat src, Rect2D roi, const vector<cv::Point> featurePoints, int minThreshold, int maxThreshold)
{
	m_createTemplateParam.srcMat = src;
	m_createTemplateParam.roi = roi;
	cv::Point center;
	int ret = m_core->CreateModel(src, featurePoints, center);
	if (ret != 0)return ret;
	m_createTemplateParam.isCreated = true;
	m_createTemplateParam.center = Point2D(center.x, center.y);
	m_createTemplateParam.featurePoints.clear();
	for each (auto var in featurePoints)
	{
		m_createTemplateParam.featurePoints.push_back(Point2D(var.x, var.y));
	}
	m_createTemplateParam.minThreshold = minThreshold;
	m_createTemplateParam.maxThreshold = maxThreshold;
	return 0;
}
int ShapeMatchOperator::createTemplate()
{
	cv::Point center;
	vector<cv::Point> points;
	for each (auto var in m_createTemplateParam.featurePoints)
	{
		points.push_back(cv::Point(var.X(), var.Y()));
	}
	return m_core->CreateModel(m_createTemplateParam.srcMat, points, center);
}
int ShapeMatchOperator::setFindTemplateParams(const double minMatchRate, const double greediness, const double startAngle, const double endAngle, const double minScale, const double maxScale, const int maxMatchNum)
{
	m_findTemplateParam.minScore = minMatchRate;
	m_findTemplateParam.greediness = greediness;
	m_findTemplateParam.minAngle = startAngle;
	m_findTemplateParam.maxAngle = endAngle;
	m_findTemplateParam.minScale = minScale;
	m_findTemplateParam.maxScale = maxScale;
	m_findTemplateParam.maxMatchNum = maxMatchNum;
	return 0;
}
void ShapeMatchOperator::setCoreParams()
{
}
int ShapeMatchOperator::findTemplate(const cv::Mat src, vector<double>& matchScore, vector<double>& matchAngle, vector<double>& matchScale, vector<cv::Point>& matchPoint, vector<vector<cv::Point>>& resultFeaturePoints, double & timeSpan)
{
	return m_core->ShapeMatch(src, matchScore, matchAngle, matchScale, matchPoint, resultFeaturePoints, timeSpan);
}
int ShapeMatchOperator::saveTemplate()
{
	string path = this->nodeDirectory().c_str();
	_mkdir(path.c_str());
	cv::imwrite(this->getTemplateImageFileName(), m_createTemplateParam.srcMat);
	std::ofstream ofs;
	ofs.open(this->getTemplateFileName().c_str(), ofstream::out);
	if (ofs)
	{
		boost::archive::text_oarchive oa(ofs);
		oa << m_createTemplateParam;
	}
	ofs.close();
	return 0;
}
int ShapeMatchOperator::loadTemplate()
{
	string path = this->getTemplateFileName();
	string::iterator it;
	for (it = path.begin();it<path.end(); it++)
	{
		if (*it == '-')
		{
			path.erase(it);
		}
	}
	std::ifstream ifs(path);
	if (ifs)
	{
		boost::archive::text_iarchive ia(ifs);
		ia >> m_createTemplateParam;
	}
	ifs.close();

	path = this->getTemplateImageFileName();
	for (it = path.begin();it<path.end(); it++)
	{
		if (*it == '-')
		{
			path.erase(it);
		}
	}
	m_createTemplateParam.srcMat = cv::imread(path, CV_8U);
	vector<cv::Point> featurePoints;
	cv::Point center;
	for each (auto var in m_createTemplateParam.featurePoints)
	{
		featurePoints.push_back(cv::Point(var.X(), var.Y()));
	}
	return m_core->CreateModel(m_createTemplateParam.srcMat, featurePoints, center);
}
void ShapeMatchOperator::deleteTemplate()
{
	m_createTemplateParam.reset();
	m_core->DeleteModel();
}
void ShapeMatchOperator::setROI(Rect2D rect)
{
	this->ROI = rect;
}
Rect2D ShapeMatchOperator::getROI()
{
	return this->ROI;
}