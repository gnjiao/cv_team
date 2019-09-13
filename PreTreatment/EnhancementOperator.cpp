#include "PreTreatment\EnhancementOperator.h"

EnhancementOperator::EnhancementOperator()
{
	m_nodeName = "enhancement";
	m_showText = "enhancement";
	m_core = new Enhancement();
}
EnhancementOperator::~EnhancementOperator()
{
	delete m_core;
	delete output_mat;
	delete output_refMat;
}
void EnhancementOperator::initOutput()
{
	output_mat = createOutput<cv::Mat>(ToolDataType::eMat);
	output_refMat = createOutput<cv::Mat>(ToolDataType::eRefMat);
	output_mat->SetShowText("Enhancement");
}
void EnhancementOperator::registerOutput()
{
	output()->Add(output_mat, ToolDataType::eMat);
	output()->Add(output_refMat, ToolDataType::eRefMat);
	output_mat->SetShowText("Enhancement");
}
NodeState EnhancementOperator::execute()
{
	if (input_mat == nullptr)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	return test(MatType::Current);
	
}
string EnhancementOperator::getResultString()
{
	string str;
	return str;
}
BaseNode * EnhancementOperator::clone()
{
	EnhancementOperator *newnode = new EnhancementOperator();

	newnode->m_mode = this->m_mode;

	newnode->input_mat = createClone<cv::Mat>(this->input_mat);
	newnode->input_refMat = createClone<cv::Mat>(this->input_refMat);

	newnode->initOutput();

	newnode->setCoreParams();
	return newnode;
}
void EnhancementOperator::resetOutputValue()
{
	if (nullptr != output_mat)
		output_mat->SetValue(cv::Mat());
	if (nullptr != output_refMat)
		output_refMat->SetValue(cv::Mat());
}
NodeState EnhancementOperator::test(MatType type)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	m_state = _test(type);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	setDuration(duration_cast<chrono::microseconds>(t2 - t1).count() / 1000.0);
	return m_state;
}
void EnhancementOperator::setCoreParams()
{
}
NodeState EnhancementOperator::_test(MatType type)
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
	case EnhancementOperator::EqualizeHist:
		m_core->EnhanceEqualizeHist(srcImage, result);
		break;
	case EnhancementOperator::Laplace:
		m_core->EnhanceLaplace(srcImage, result);
		break;
	case EnhancementOperator::Gamma:
		m_core->EnhanceGamma(srcImage, result);
		break;
	case EnhancementOperator::Log:
		m_core->EnhanceLog(srcImage, result);
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