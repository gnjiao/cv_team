#include "Recognition\BarcodeOperator.h"

BarcodeOperator::BarcodeOperator()
{
	m_nodeName = "Barcode";
	m_showText = "Barcode";
	m_core = new Barcode();
}
BarcodeOperator::~BarcodeOperator()
{
	delete m_core;
	delete output_string;
}
void BarcodeOperator::initOutput()
{
	output_string = createOutput<string>(ToolDataType::eString);
	output_string->SetDisplayEnabled(true);
	//output_refMat = createOutput<cv::Mat>(ToolDataType::eRefMat);
}
void BarcodeOperator::registerOutput()
{
	output()->Add(output_string, ToolDataType::eString);
	output_string->SetShowText("Barcode");
}
NodeState BarcodeOperator::execute()
{
	if (input_mat == nullptr)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	return test(MatType::Current);
}
string BarcodeOperator::getResultString()
{
	string str;
	return str;
}
BaseNode * BarcodeOperator::clone()
{
	BarcodeOperator *newnode = new BarcodeOperator();
	newnode->input_mat = createClone<cv::Mat>(this->input_mat);
	newnode->input_refMat = createClone<cv::Mat>(this->input_refMat);
	newnode->initOutput();
	newnode->setCoreParams();
	return newnode;
}
void BarcodeOperator::resetOutputValue()
{
	if (nullptr != output_string)
		output_string->SetValue("");
	//if (nullptr != output_refMat)
	// output_refMat->SetValue(cv::Mat());
}
NodeState BarcodeOperator::test(MatType type)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	m_state = _test(type);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	setDuration(duration_cast<chrono::microseconds>(t2 - t1).count() / 1000.0);
	return m_state;
}
void BarcodeOperator::setCoreParams()
{
}
NodeState BarcodeOperator::_test(MatType type)
{
	cv::Mat src;
	string result;
	string code_type;
	src = input_mat->GetValue();
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
	int ret = m_core->DecodeQR(srcImage, code_type, result);
	if (!ret)
	{
		output_string->SetValue(code_type + result);
	}
	else
	{
		return NodeState::Failure;
	}
	return NodeState::Success;
}