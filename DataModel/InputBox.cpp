#include "DataModel\InputBox.h"



InputBox::InputBox(QWidget *parent) : QComboBox(parent)

{

}



InputBox::~InputBox()

{

}



InputBox * InputBox::setInputData(InputData data)

{

	m_inputData = data;

	return this;

}

InputBox * InputBox::setDataType(ToolDataType dataType)
{
	m_dataType = dataType;
	return this;
}



InputBox * InputBox::loadItems()

{

	this->clear();

	switch (m_dataType)

	{

	case toolModel::eInt:

		for each (auto var in m_inputData.vectorInt)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eDouble:

		for each (auto var in m_inputData.vectorDouble)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eBool:

		for each (auto var in m_inputData.vectorBool)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eString:

		for each (auto var in m_inputData.vectorString)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eMat:

		for each (auto var in m_inputData.vectorMat)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::ePoint:

		for each (auto var in m_inputData.vectorPoint)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eLine:

		for each (auto var in m_inputData.vectorLine)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eCircle:

		for each (auto var in m_inputData.vectorCircle)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eLength:

		for each (auto var in m_inputData.vectorLength)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eArea:

		for each (auto var in m_inputData.vectorArea)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eAngle:

		for each (auto var in m_inputData.vectorAngle)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eScore:

		for each (auto var in m_inputData.vectorScore)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	case toolModel::eAffineTrans:

		for each (auto var in m_inputData.vectorAffineTrans)

		{

			this->addItem(QString::fromStdString(var->GetNodeShowText()));

		}

		break;

	default:

		break;

	}

	return this;

}



void InputBox::selectItem(ToolData<int>* toolData)

{

	if (toolData == nullptr)return;

	for (int i = 0; i < m_inputData.vectorInt.size(); i++)

	{

		if (toolData->GetNodeKey() != m_inputData.vectorInt.at(i)->GetNodeKey())

			continue;

		if (toolData->GetIndex() != m_inputData.vectorInt.at(i)->GetIndex())

			continue;

		this->setCurrentIndex(i);

	}

}



void InputBox::selectItem(ToolData<double>* toolData)

{

	if (toolData == nullptr)return;

	switch (m_dataType)

	{

	case toolModel::eDouble:

		for (int i = 0; i < m_inputData.vectorDouble.size(); i++)

		{

			if (toolData->GetNodeKey() != m_inputData.vectorDouble.at(i)->GetNodeKey())

				continue;

			if (toolData->GetIndex() != m_inputData.vectorDouble.at(i)->GetIndex())

				continue;

			this->setCurrentIndex(i);

		}

		break;

	case toolModel::eLength:

		for (int i = 0; i < m_inputData.vectorLength.size(); i++)

		{

			if (toolData->GetNodeKey() != m_inputData.vectorLength.at(i)->GetNodeKey())

				continue;

			if (toolData->GetIndex() != m_inputData.vectorLength.at(i)->GetIndex())

				continue;

			this->setCurrentIndex(i);

		}

		break;

	case toolModel::eArea:

		for (int i = 0; i < m_inputData.vectorArea.size(); i++)

		{

			if (toolData->GetNodeKey() != m_inputData.vectorArea.at(i)->GetNodeKey())

				continue;

			if (toolData->GetIndex() != m_inputData.vectorArea.at(i)->GetIndex())

				continue;

			this->setCurrentIndex(i);

		}

		break;

	case toolModel::eScore:

		for (int i = 0; i < m_inputData.vectorScore.size(); i++)

		{

			if (toolData->GetNodeKey() != m_inputData.vectorScore.at(i)->GetNodeKey())

				continue;

			if (toolData->GetIndex() != m_inputData.vectorScore.at(i)->GetIndex())

				continue;

			this->setCurrentIndex(i);

		}

		break;

	}

}



void InputBox::selectItem(ToolData<bool>* toolData)

{

	if (toolData == nullptr)return;

	for (int i = 0; i < m_inputData.vectorBool.size(); i++)

	{

		if (toolData->GetNodeKey() != m_inputData.vectorBool.at(i)->GetNodeKey())

			continue;

		if (toolData->GetIndex() != m_inputData.vectorBool.at(i)->GetIndex())

			continue;

		this->setCurrentIndex(i);

	}

}



void InputBox::selectItem(ToolData<string>* toolData)

{

	if (toolData == nullptr)return;

	for (int i = 0; i < m_inputData.vectorString.size(); i++)

	{

		if (toolData->GetNodeKey() != m_inputData.vectorString.at(i)->GetNodeKey())

			continue;

		if (toolData->GetIndex() != m_inputData.vectorString.at(i)->GetIndex())

			continue;

		this->setCurrentIndex(i);

	}

}



void InputBox::selectItem(ToolData<cv::Mat>* toolData)
{

	if (toolData == nullptr)return;

	for (int i = 0; i < m_inputData.vectorMat.size(); i++)

	{

		if (toolData->GetNodeKey() != m_inputData.vectorMat.at(i)->GetNodeKey())

			continue;

		if (toolData->GetIndex() != m_inputData.vectorMat.at(i)->GetIndex())

			continue;

		this->setCurrentIndex(i);

	}

}



void InputBox::selectItem(ToolData<Point2D>* toolData)

{

	if (toolData == nullptr)return;

	for (int i = 0; i < m_inputData.vectorPoint.size(); i++)

	{

		if (toolData->GetNodeKey() != m_inputData.vectorPoint.at(i)->GetNodeKey())

			continue;

		if (toolData->GetIndex() != m_inputData.vectorPoint.at(i)->GetIndex())

			continue;

		this->setCurrentIndex(i);

	}

}



void InputBox::selectItem(ToolData<Line2D>* toolData)

{

	if (toolData == nullptr)return;

	for (int i = 0; i < m_inputData.vectorLine.size(); i++)

	{

		if (toolData->GetNodeKey() != m_inputData.vectorLine.at(i)->GetNodeKey())

			continue;

		if (toolData->GetIndex() != m_inputData.vectorLine.at(i)->GetIndex())

			continue;

		this->setCurrentIndex(i);

	}

}



void InputBox::selectItem(ToolData<Circle2D>* toolData)

{

	if (toolData == nullptr)return;

	for (int i = 0; i < m_inputData.vectorCircle.size(); i++)

	{

		if (toolData->GetNodeKey() != m_inputData.vectorCircle.at(i)->GetNodeKey())

			continue;

		if (toolData->GetIndex() != m_inputData.vectorCircle.at(i)->GetIndex())

			continue;

		this->setCurrentIndex(i);

	}

}



void InputBox::selectItem(ToolData<Angle>* toolData)

{

	if (toolData == nullptr)return;

	for (int i = 0; i < m_inputData.vectorAngle.size(); i++)

	{

		if (toolData->GetNodeKey() != m_inputData.vectorAngle.at(i)->GetNodeKey())

			continue;

		if (toolData->GetIndex() != m_inputData.vectorAngle.at(i)->GetIndex())

			continue;

		this->setCurrentIndex(i);

	}

}



void InputBox::selectItem(ToolData<AffineTrans>* toolData)

{

	if (toolData == nullptr)return;

	for (int i = 0; i < m_inputData.vectorAffineTrans.size(); i++)

	{

		if (toolData->GetNodeKey() != m_inputData.vectorAffineTrans.at(i)->GetNodeKey())

			continue;

		if (toolData->GetIndex() != m_inputData.vectorAffineTrans.at(i)->GetIndex())

			continue;

		this->setCurrentIndex(i);

	}

}



ToolData<int>* InputBox::getInt(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getInt(index);

}



ToolData<double>* InputBox::getDouble(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getDouble(index);

}



ToolData<bool>* InputBox::getBool(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getBool(index);

}



ToolData<string>* InputBox::getString(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getString(index);

}



ToolData<cv::Mat>* InputBox::getMat(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getMat(index);

}



ToolData<Point2D>* InputBox::getPoint(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getPoint(index);

}



ToolData<Line2D>* InputBox::getLine(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getLine(index);

}



ToolData<Circle2D>* InputBox::getCircle(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getCircle(index);

}



ToolData<double>* InputBox::getLength(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getLength(index);

}



ToolData<double>* InputBox::getArea(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getArea(index);

}



ToolData<Angle>* InputBox::getAngle(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getAngle(index);

}



ToolData<double>* InputBox::getScore(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getScore(index);

}



ToolData<AffineTrans>* InputBox::getAffineTrans(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getAffineTrans(index);

}



ToolData<cv::Mat>* InputBox::getRefMat(int index)

{

	if (index < 0)

	{

		return nullptr;

	}

	return m_inputData.getRefMat(index);

}