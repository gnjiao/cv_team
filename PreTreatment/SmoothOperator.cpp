#include "PreTreatment\SmoothOperator.h"

SmoothOperator::SmoothOperator()
{
	m_nodeName = "smooth";
	m_showText = "smooth";
	m_core = new Smooth();
}
SmoothOperator::~SmoothOperator()
{
	delete m_core;
	delete output_mat;
	delete output_refMat;
}
void SmoothOperator::initOutput()
{
	output_mat = createOutput<cv::Mat>(ToolDataType::eMat);
	output_refMat = createOutput<cv::Mat>(ToolDataType::eRefMat);
	output_mat->SetShowText("Smooth");
}
void SmoothOperator::registerOutput()
{
	output()->Add(output_mat, ToolDataType::eMat);
	output()->Add(output_refMat, ToolDataType::eRefMat);
	output_mat->SetShowText("Smooth");
}
NodeState SmoothOperator::execute()
{
	if (input_mat == nullptr)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	return test(MatType::Current);
}
string SmoothOperator::getResultString()
{
	string str;
	return str;
}
BaseNode * SmoothOperator::clone()
{
	SmoothOperator *newnode = new SmoothOperator();
	newnode->m_mode = this->m_mode;
	newnode->m_blurSmoothParam = this->m_blurSmoothParam;
	newnode->m_gaussianSmoothParam = this->m_gaussianSmoothParam;
	newnode->m_boxSmoothParam = this->m_boxSmoothParam;
	newnode->m_bilateralSmoothParam = this->m_bilateralSmoothParam;
	newnode->m_medianSmoothParam = this->m_medianSmoothParam;
	newnode->input_mat = createClone<cv::Mat>(this->input_mat);
	newnode->input_refMat = createClone<cv::Mat>(this->input_refMat);
	newnode->initOutput();
	newnode->setCoreParams();
	return newnode;
}
void SmoothOperator::resetOutputValue()
{
	if (output_mat != nullptr)
		output_mat->SetValue(cv::Mat());
	if (nullptr != output_refMat)
		output_refMat->SetValue(cv::Mat());
}
NodeState SmoothOperator::test(MatType type)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	m_state = _test(type);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	setDuration(duration_cast<chrono::microseconds>(t2 - t1).count() / 1000.0);
	return m_state;
}
void SmoothOperator::setCoreParams()
{
}
NodeState SmoothOperator::_test(MatType type)
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
		NodeState::Failure;
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
	case SmoothOperator::Blur:
		m_core->BlurSmooth(srcImage, result, m_blurSmoothParam.kernelSize);
		break;
	case SmoothOperator::Gaussian:
		m_core->GaussianSmooth(srcImage, result, m_gaussianSmoothParam.kernelSize,
			m_gaussianSmoothParam.sigmaX, m_gaussianSmoothParam.sigmaY);
		break;
	case SmoothOperator::Box:
		m_core->BoxSmooth(srcImage, result,
			m_boxSmoothParam.kernelSize, m_boxSmoothParam.normalized);
		break;
	case SmoothOperator::Bilateral:
		m_core->BilateralSmooth(srcImage, result, m_bilateralSmoothParam.sigmaColor,
			m_bilateralSmoothParam.sigmaSpace, m_bilateralSmoothParam.d);
		break;
	case SmoothOperator::Median:
		m_core->MedianSmooth(srcImage, result, m_medianSmoothParam.kernelSize);
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