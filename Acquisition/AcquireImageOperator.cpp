#include "AcquireImageOperator.h"


AcquireImageOperator::AcquireImageOperator()
{
	m_nodeName = "AcquireImage";
	m_showText = "AcquireImage";
}
AcquireImageOperator::~AcquireImageOperator()
{
	delete output_mat;
	delete output_refMat;
	delete output_nullAffineTrans;
}
void AcquireImageOperator::initOutput()
{
	output_mat = createOutput<cv::Mat>(ToolDataType::eMat);
	output_refMat = createOutput<cv::Mat>(ToolDataType::eRefMat);
	output_nullAffineTrans = createOutput<AffineTrans>(ToolDataType::eAffineTrans);
	output_mat->SetShowText("Input Mat");
	output_refMat->SetShowText("Reffrence Mat");
	output_nullAffineTrans->SetShowText("AffineTrans");
}
void AcquireImageOperator::registerOutput()
{
	output()->Add(output_mat, ToolDataType::eMat);
	output()->Add(output_refMat, ToolDataType::eRefMat);
	output()->Add(output_nullAffineTrans, ToolDataType::eAffineTrans);
	output_mat->SetShowText("Mat");
	output_nullAffineTrans->SetShowText("AffineTrans");
}
NodeState AcquireImageOperator::execute()
{
	Inspection* inspection = InspectionMGR->FindBy(this->getInspectionKey());
	if (inspection == nullptr)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	output_mat->SetValue(inspection->GetAcquisition()->GetCurrentMat());
	inspection = nullptr;
	m_state = NodeState::Success;
	return NodeState::Success;
}
string AcquireImageOperator::getResultString()
{
	string str;
	return str;
}
BaseNode * AcquireImageOperator::clone()
{
	AcquireImageOperator *newnode = new AcquireImageOperator();
	newnode->initOutput();
	newnode->setCoreParams();
	return newnode;
}
void AcquireImageOperator::resetOutputValue()
{
	if (nullptr != output_mat)
		output_mat->SetValue(cv::Mat());
	if (nullptr != output_refMat)
		output_refMat->SetValue(cv::Mat());
}
void AcquireImageOperator::setCoreParams()
{
	Inspection* inspection = InspectionMGR->FindBy(this->getInspectionKey());
	if (inspection == nullptr)
		return;
	output_mat->SetValue(inspection->GetAcquisition()->GetCurrentMat());
	output_refMat->SetValue(inspection->GetAcquisition()->GetReferenceMat());
	inspection = nullptr;		

}