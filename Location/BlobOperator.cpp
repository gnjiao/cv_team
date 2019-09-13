#include "BlobOperator.h"

using namespace SIMPLEBLOB_SPACE;

BlobOperator::BlobOperator()
{
	m_nodeName = "blob";
	m_showText = "blob";
	pblob = new SimpleBlob;
	initOutput();
}
BlobOperator::~BlobOperator()
{
	delete pblob;
	delete output_point;
	delete output_area;
	delete output_affineTrans;
	delete output_region;
	pblob = nullptr;
	output_point = nullptr;
	output_area = nullptr;
	output_affineTrans = nullptr;
	output_region = nullptr;

}
void BlobOperator::initOutput()
{
	output_point = createOutput<Point2D>(ToolDataType::ePoint)->SetDisplayEnabled(true);
	output_area = createOutput<double>(ToolDataType::eArea)->SetValue(0);
	output_affineTrans = createOutput<AffineTrans>(ToolDataType::eAffineTrans);
	output_region = createOutput<cv::Mat>(ToolDataType::eRegion);
	//output_point->SetShowText(TOOLBOXMGR->blob.map[TOOLBOXMGR->blob.center]);
	//output_area->SetShowText(TOOLBOXMGR->blob.map[TOOLBOXMGR->blob.area]);
	//output_affineTrans->SetShowText(TOOLBOXMGR->blob.map[TOOLBOXMGR->blob.affineTrans]);
	//output_region->SetShowText(TOOLBOXMGR->blob.map[TOOLBOXMGR->blob.region]);
	//output_region->SetDisplayEnabled(true);
}
void BlobOperator::registerOutput()
{
	output()->Add(output_point, ToolDataType::ePoint);
	output()->Add(output_area, ToolDataType::eArea);
	//output_point->SetShowText(TOOLBOXMGR->blob.map[TOOLBOXMGR->blob.center]);
	//output_area->SetShowText(TOOLBOXMGR->blob.map[TOOLBOXMGR->blob.area]);
	if (nullptr == output_affineTrans)
	{
		output_affineTrans = createOutput<AffineTrans>(ToolDataType::eAffineTrans);
		//output_affineTrans->SetShowText(TOOLBOXMGR->blob.map[TOOLBOXMGR->blob.affineTrans]);
		output_affineTrans->SetInspectionKey(output_point->GetInspectionKey());
		output_affineTrans->SetNodeKey(output_point->GetNodeKey());
	}
	//output_affineTrans->SetShowText(TOOLBOXMGR->blob.map[TOOLBOXMGR->blob.affineTrans]);
	output()->Add(output_affineTrans, ToolDataType::eAffineTrans);

	//output_region->SetShowText(TOOLBOXMGR->blob.map[TOOLBOXMGR->blob.region]);
	output()->Add(output_region, ToolDataType::eRegion);
}
NodeState BlobOperator::execute()
{
	output_area->SetValue(0);
	output_point->SetValue(Point2D());
	AffineTrans tempaffineTrans = output_affineTrans->GetValue();
	tempaffineTrans.Translation = Point2D();
	output_affineTrans->SetValue(tempaffineTrans);
	if (input_mat == nullptr)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	m_state = NodeState::Running;
	return test(input_mat->GetValue());
}
string BlobOperator::getResultString()
{
	string str;
	if (output_point->GetOutputEnabled() && output_point->GetOutputChecked())
	{
		str += BaseOperator::transPoint2D(output_point->GetValue()).ToString();
	}
	if (output_area->GetOutputEnabled() && output_area->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += to_string(BaseOperator::transValue(output_area->GetValue()));
	}
	return str;
}
BaseNode * BlobOperator::clone()
{
	BlobOperator *newnode = new BlobOperator();
	newnode->setParams(this->getParameters());
	newnode->input_mat = createClone<cv::Mat>(this->input_mat);
	newnode->input_refMat = createClone<cv::Mat>(this->input_refMat);
	newnode->initOutput();
	setCoreParams();
	return newnode;
}
void BlobOperator::resetOutputValue()
{
	if (nullptr != output_point)
		output_point->SetValue(Point2D());
	if (nullptr != output_area)
		output_area->SetValue(0);
}
void BlobOperator::drawFeatures(cv::Mat mat)
{
	pblob->Draw_results(mat, 0);
}
NodeState BlobOperator::test(cv::Mat src)
{
	cv::Mat srcImage;
	if (src.channels() > 1)
	{
		cv::cvtColor(src, srcImage, cv::COLOR_RGB2GRAY);
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
void BlobOperator::setParams(SIMPLEBLOB_SPACE::Params params)
{
	m_params = params;
	setCoreParams();
}
void BlobOperator::setCoreParams()
{
	this->pblob->set_params(m_params);
}
SIMPLEBLOB_SPACE::Params BlobOperator::getParameters() const
{
	return m_params;
}
NodeState BlobOperator::_test(cv::Mat src)
{
	if (!src.data)
	{
		return NodeState::Failure;
	}
	//if (roiChecked)
	//{
	//	cv::Rect roi(this->ROI.LeftTop().X(), this->ROI.LeftTop().Y(), this->ROI.Length(), this->ROI.Width());
	//	pblob->findBlobs(src, roi);
	//}
	//else
	//{
		pblob->findBlobs(src);
	//}
	if (0 >= pblob->m_result.size())
	{
		return NodeState::Failure;
	}
	output_point->SetValue(Point2D(pblob->m_result[0].location.x, pblob->m_result[0].location.y));
	output_area->SetValue(pblob->m_result[0].area);
	AffineTrans temp = output_affineTrans->GetValue();
	Point2D center_org(m_params.center_x, m_params.center_y);
	temp.Translation = output_point->GetValue() - center_org;
	output_affineTrans->SetValue(temp);
	return NodeState::Success;
}