#include "PreTreatment\ThresholdOperator.h"


ThresholdOperator::ThresholdOperator()
{
	m_nodeName = "threshold";
	m_showText = "threshold";
	m_threshold = new Threshold();
}
ThresholdOperator::~ThresholdOperator()
{
	delete m_threshold;
	delete output_mat;
	delete output_refMat;
}
void ThresholdOperator::initOutput()
{
	output_mat = createOutput<cv::Mat>(ToolDataType::eMat);
	output_refMat = createOutput<cv::Mat>(ToolDataType::eRefMat);
	output_mat->SetShowText("Threshold");
}
void ThresholdOperator::registerOutput()
{
	output()->Add(output_mat, ToolDataType::eMat);
	output()->Add(output_refMat, ToolDataType::eRefMat);
	output_mat->SetShowText("Threshold");
}
NodeState ThresholdOperator::execute()
{
	if (input_mat == nullptr)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	return test(MatType::Current);
}
string ThresholdOperator::getResultString()
{
	string str;
	return str;
}
BaseNode* ThresholdOperator::clone()
{
	ThresholdOperator *newnode = new ThresholdOperator();
	newnode->m_mode = this->m_mode;
	newnode->m_fixParam = this->m_fixParam;
	newnode->m_autoParam = this->m_autoParam;
	newnode->m_segmentParam = this->m_segmentParam;
	newnode->m_twoPeakParam = this->m_twoPeakParam;
	newnode->input_mat = createClone<cv::Mat>(this->input_mat);
	newnode->input_refMat = createClone<cv::Mat>(this->input_refMat);
	newnode->initOutput();
	newnode->setCoreParams();
	return newnode;
}
void ThresholdOperator::resetOutputValue()
{
	if (output_mat != nullptr)
		output_mat->SetValue(cv::Mat());
	if (nullptr != output_refMat)
		output_refMat->SetValue(cv::Mat());
}
NodeState ThresholdOperator::test(MatType type)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	m_state = _test(type);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	setDuration(duration_cast<chrono::microseconds>(t2 - t1).count() / 1000.0);
	return m_state;
}
void ThresholdOperator::setCoreParams()
{
}
NodeState ThresholdOperator::_test(MatType type)
{
	cv::Mat src;
	cv::Mat result;
	switch (type)
	{
	case toolModel::Reference:
		src = input_refMat->GetValue();
		break;
	case toolModel::Current:
		src = input_mat->GetValue();
		break;
	}
	if (!src.data)
	{
		return NodeState::Failure;
	}
	cv::Mat srcImage;
	if (src.channels() > 1)
	{
		cv::cvtColor(src, srcImage, cv::COLOR_RGB2GRAY);
	}
	else
	{
		src.copyTo(srcImage);
	}
	switch (m_mode)
	{
	case ThresholdOperator::ThresholdMode::Fix:
		m_threshold->ThresholdFix(srcImage, result,
			m_fixParam.thresholdValue, m_fixParam.maxValue, m_fixParam.fixType);
		break;
	case ThresholdOperator::ThresholdMode::Auto:
		m_threshold->ThresholdAuto(srcImage, result,
			m_autoParam.maxValue, m_autoParam.autoType, m_autoParam.fixType, m_autoParam.blockSize, m_autoParam.C);
		break;
	case ThresholdOperator::ThresholdMode::Segment:
		m_threshold->ThresholdSegment(srcImage, result,
			m_segmentParam.baseValue, m_segmentParam.maxValue);
		break;
	case ThresholdOperator::ThresholdMode::TwoPeak:
		m_threshold->ThresholdTwoPeak(srcImage, result,
			m_twoPeakParam.maxValue, m_twoPeakParam.fixType);
		break;
	}
	switch (type)
	{
	case toolModel::Reference:
		output_refMat->SetValue(result);
		break;
	case toolModel::Current:
		output_mat->SetValue(result);
		break;
	}
	return NodeState::Success;
}