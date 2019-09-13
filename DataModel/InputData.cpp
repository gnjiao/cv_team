#include "DataModel\InputData.h"


InputData::InputData()
{
}
InputData::~InputData()
{
}
InputData InputData::append(InputData data)
{
	for each (auto var in data.vectorAffineTrans)
	{
		this->vectorAffineTrans.push_back(var);
	}
	for each (auto var in data.vectorAngle)
	{
		this->vectorAngle.push_back(var);
	}
	for each (auto var in data.vectorArea)
	{
		this->vectorArea.push_back(var);
	}
	for each (auto var in data.vectorBool)
	{
		this->vectorBool.push_back(var);
	}
	for each (auto var in data.vectorCircle)
	{
		this->vectorCircle.push_back(var);
	}
	for each (auto var in data.vectorDouble)
	{
		this->vectorDouble.push_back(var);
	}
	for each (auto var in data.vectorInt)
	{
		this->vectorInt.push_back(var);
	}
	for each (auto var in data.vectorLength)
	{
		this->vectorLength.push_back(var);
	}
	for each (auto var in data.vectorLine)
	{
		this->vectorLine.push_back(var);
	}
	for each (auto var in data.vectorMat)
	{
		this->vectorMat.push_back(var);
	}
	for each (auto var in data.vectorPoint)
	{
		this->vectorPoint.push_back(var);
	}
	for each (auto var in data.vectorRefMat)
	{
		this->vectorRefMat.push_back(var);
	}
	for each (auto var in data.vectorScore)
	{
		this->vectorScore.push_back(var);
	}
	for each (auto var in data.vectorString)
	{
		this->vectorString.push_back(var);
	}
	return *this;
}
ToolData<int>* InputData::getInt(int index)
{
	if (vectorInt.size() < index + 1)
		return nullptr;
	return vectorInt.at(index);
}
ToolData<double>* InputData::getDouble(int index)
{
	if (vectorDouble.size() < index + 1)
		return nullptr;
	return vectorDouble.at(index);
}
ToolData<bool>* InputData::getBool(int index)
{
	if (vectorBool.size() < index + 1)
		return nullptr;
	return vectorBool.at(index);
}
ToolData<string>* InputData::getString(int index)
{
	if (vectorString.size() < index + 1)
		return nullptr;
	return vectorString.at(index);
}
ToolData<cv::Mat>* InputData::getMat(int index)
{
	if (vectorMat.size() < index + 1)
		return nullptr;
	return vectorMat.at(index);
}
ToolData<Point2D>* InputData::getPoint(int index)
{
	if (vectorPoint.size() < index + 1)
		return nullptr;
	return vectorPoint.at(index);
}
ToolData<Line2D>* InputData::getLine(int index)
{
	if (vectorLine.size() < index + 1)
		return nullptr;
	return vectorLine.at(index);
}
ToolData<Circle2D>* InputData::getCircle(int index)
{
	if (vectorCircle.size() < index + 1)
		return nullptr;
	return vectorCircle.at(index);
}
ToolData<double>* InputData::getLength(int index)
{
	if (vectorLength.size() < index + 1)
		return nullptr;
	return vectorLength.at(index);
}
ToolData<double>* InputData::getArea(int index)
{
	if (vectorArea.size() < index + 1)
		return nullptr;
	return vectorArea.at(index);
}
ToolData<Angle>* InputData::getAngle(int index)
{
	if (vectorAngle.size() < index + 1)
		return nullptr;
	return vectorAngle.at(index);
}
ToolData<double>* InputData::getScore(int index)
{
	if (vectorScore.size() < index + 1)
		return nullptr;
	return vectorScore.at(index);
}
ToolData<AffineTrans>* InputData::getAffineTrans(int index)
{
	if (vectorAffineTrans.size() < index + 1)
		return nullptr;
	return vectorAffineTrans.at(index);
}
ToolData<cv::Mat>* InputData::getRefMat(int index)
{
	if (vectorRefMat.size() < index + 1)
		return nullptr;
	return vectorRefMat.at(index);
}