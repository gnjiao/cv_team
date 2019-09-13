#include "PreTreatment\MorphologyOperator.h"

MorphologyOperator::MorphologyOperator()
{
	m_nodeName = "morphology";
	m_showText = "morphology";
	m_core = new Morphology();
}
MorphologyOperator::~MorphologyOperator()
{
	delete m_core;
	delete output_mat;
	delete output_refMat;
}
void MorphologyOperator::initOutput()
{
	output_mat = createOutput<cv::Mat>(ToolDataType::eMat);
	output_refMat = createOutput<cv::Mat>(ToolDataType::eRefMat);
	//output_mat->SetShowText(TOOLBOXMGR->morphology.map[TOOLBOXMGR->morphology.outputImage]);
}
void MorphologyOperator::registerOutput()
{
	output()->Add(output_mat, ToolDataType::eMat);
	output()->Add(output_refMat, ToolDataType::eRefMat);
	//output_mat->SetShowText(TOOLBOXMGR->morphology.map[TOOLBOXMGR->morphology.outputImage]);
}
NodeState MorphologyOperator::execute()
{
	if (input_mat == nullptr)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	return test(MatType::Current);
}
string MorphologyOperator::getResultString()
{
	string str;
	return str;
}
BaseNode * MorphologyOperator::clone()
{
	MorphologyOperator *newnode = new MorphologyOperator();

	newnode->m_mode = this->m_mode;
	newnode->m_dilateParam = this->m_dilateParam;
	newnode->m_erodeParam = this->m_erodeParam;
	newnode->m_advancedParam = this->m_advancedParam;
	newnode->input_mat = createClone<cv::Mat>(this->input_mat);
	newnode->input_refMat = createClone<cv::Mat>(this->input_refMat);
	newnode->initOutput();
	newnode->setCoreParams();
	return newnode;
}
void MorphologyOperator::resetOutputValue()
{
	if (nullptr != output_mat)
		output_mat->SetValue(cv::Mat());
	if (nullptr != output_refMat)
		output_refMat->SetValue(cv::Mat());
}
NodeState MorphologyOperator::test(MatType type)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	m_state = _test(type);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	setDuration(duration_cast<chrono::microseconds>(t2 - t1).count() / 1000.0);
	return m_state;
}
void MorphologyOperator::setCoreParams()
{
}
NodeState MorphologyOperator::_test(MatType type)
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
	case MorphologyOperator::Dilate:
		m_core->SetStructurElementShape(m_dilateParam.structElementShape);
		m_core->Dilate(srcImage, result, m_dilateParam.structElementSize, m_dilateParam.iteration);
		break;
	case MorphologyOperator::Erode:
		m_core->SetStructurElementShape(m_erodeParam.structElementShape);
		m_core->Erode(srcImage, result, m_erodeParam.structElementSize, m_erodeParam.iteration);
		break;
	case MorphologyOperator::Advanced:
		m_core->SetStructurElementShape(m_advancedParam.structElementShape);
		m_core->MorphologyOperator(srcImage, result,
			(cv::MorphTypes)m_advancedParam.morphType, m_advancedParam.structElementSize, m_advancedParam.iteration);
		break;
	default:
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