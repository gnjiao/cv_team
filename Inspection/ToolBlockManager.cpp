#include "ToolBlockManager.h"
#include <boost\archive\text_oarchive.hpp>
#include <boost\archive\text_iarchive.hpp>
#include <boost\serialization\vector.hpp>
#include <boost\serialization\export.hpp>
BOOST_CLASS_EXPORT(ToolBlock)
BOOST_CLASS_EXPORT(AcquireImageOperator)
BOOST_CLASS_EXPORT(FindLineOperator)
BOOST_CLASS_EXPORT(FindCircleOperator)
BOOST_CLASS_EXPORT(ShapeMatchOperator)
BOOST_CLASS_EXPORT(EnhancementOperator)
BOOST_CLASS_EXPORT(MorphologyOperator)
BOOST_CLASS_EXPORT(SmoothOperator)
BOOST_CLASS_EXPORT(ThresholdOperator)
BOOST_CLASS_EXPORT(BarcodeOperator)
//
ToolBlockManager::ToolBlockManager()
{
}
ToolBlockManager::~ToolBlockManager()
{
}
void ToolBlockManager::setInspection(Inspection * inspection)
{
	m_inspection = inspection;
	this->m_toolBlock = inspection->m_toolBlock;
}
Inspection * ToolBlockManager::getInspection() const
{
	return this->m_inspection;
}
ToolBlock * ToolBlockManager::getToolBlock() const
{
	return this->m_toolBlock;
}
void ToolBlockManager::save()
{
	if (m_inspection == nullptr)
	{
		return;
	}
	m_inspection->MakeDir();
	string fileName = m_inspection->GetToolBlockFileName();
	std::ofstream ofs;
	ofs.open(fileName.c_str(), ofstream::out);
	if (ofs)
	{
		boost::archive::text_oarchive oa(ofs);
		oa << m_toolBlock;
	}
	ofs.close();
}
ToolBlock * ToolBlockManager::load(string m_fileName)
{
	ToolBlock* data = nullptr;
	std::ifstream ifs(m_fileName);
	if (ifs)
	{
		boost::archive::text_iarchive ia(ifs);
		ia >> data;
	}
	ifs.close();
	return data;
}
InputData ToolBlockManager::loadOperators(int index, ToolDataType dataType)
{
	InputData data;
	for (int i = 0; i < index; i++)
	{
		BaseOperator* shell = (BaseOperator*)m_toolBlock->GetChild(i);
		switch (dataType)
		{
		case toolModel::eInt:
			for each (auto var in shell->output()->vectorInt)
			{
				data.vectorInt.push_back(var);
			}
			break;
		case toolModel::eDouble:
			for each (auto var in shell->output()->vectorDouble)
			{
				data.vectorDouble.push_back(var);
			}
			break;
		case toolModel::eBool:
			for each (auto var in shell->output()->vectorBool)
			{
				data.vectorBool.push_back(var);
			}
			break;
		case toolModel::eString:
			for each (auto var in shell->output()->vectorString)
			{
				data.vectorString.push_back(var);
			}
			break;
		case toolModel::eMat:
			for each (auto var in shell->output()->vectorMat)
			{
				data.vectorMat.push_back(var);
			}
			break;
		case toolModel::ePoint:
			for each (auto var in shell->output()->vectorPoint)
			{
				data.vectorPoint.push_back(var);
			}
			break;
		case toolModel::eLine:
			for each (auto var in shell->output()->vectorLine)
			{
				data.vectorLine.push_back(var);
			}
			break;
		case toolModel::eCircle:
			for each (auto var in shell->output()->vectorCircle)
			{
				data.vectorCircle.push_back(var);
			}
			break;
		case toolModel::eLength:
			for each (auto var in shell->output()->vectorLength)
			{
				data.vectorLength.push_back(var);
			}
			break;
		case toolModel::eArea:
			for each (auto var in shell->output()->vectorArea)
			{
				data.vectorArea.push_back(var);
			}
			break;
		case toolModel::eAngle:
			for each (auto var in shell->output()->vectorAngle)
			{
				data.vectorAngle.push_back(var);
			}
			break;
		case toolModel::eScore:
			for each (auto var in shell->output()->vectorScore)
			{
				data.vectorScore.push_back(var);
			}
		case toolModel::eAffineTrans:
			for each (auto var in shell->output()->vectorAffineTrans)
			{
				data.vectorAffineTrans.push_back(var);
			}
			break;
		case toolModel::eRefMat:
			for each (auto var in shell->output()->vectorRefMat)
			{
				data.vectorRefMat.push_back(var);
			}
			break;
		default:
			break;
		}
	}
	return data;
}
void ToolBlockManager::initOperator(int index, bool isWidgetShow, vector<QDialog*>* OpratorWidget)
{
	if (m_inspection == nullptr)
		return;
	if (isWidgetShow)
		if (index < 0 || index >= OpratorWidget->size() || nullptr == OpratorWidget->at(index))
			return;
	BaseOperator* baseTool = (BaseOperator*)this->m_toolBlock->GetChild(index);
	QString title = QString::fromStdString(m_inspection->GetName()) + " -> "
		+ QString::fromStdString(baseTool->GetNodeShowText())
		+ QString::number(baseTool->GetNodeKey());
	initDisplay(false);
	if (baseTool->GetNodeName() == "AcquireImage")
	{
		AcquireImageOperator* shell = (AcquireImageOperator*)baseTool;
		Inspection *inspection = InspectionMGR->FindBy(shell->getInspectionKey());
		shell->setCoreParams();
		if (isWidgetShow)
		{
			AcquireImageWidget* shellWidget = (AcquireImageWidget*)(OpratorWidget->at(index));
			shellWidget->setWindowTitle(title);
			shellWidget->setOperator(shell);
			shellWidget->hide();
			shellWidget->raise();
			shellWidget->show();
			return;
		}
	}
	if (baseTool->GetNodeName() == "findLine")
	{
		FindLineOperator* shell = (FindLineOperator*)baseTool;
		InputData data;
		data.append(loadOperators(index, ToolDataType::eMat));
		data.append(loadOperators(index, ToolDataType::eRefMat));
		data.append(loadOperators(index, ToolDataType::eAffineTrans));
		if (shell->input_mat == nullptr)
		{
			shell->input_mat = data.getMat(0);
		}
		if (shell->input_refMat == nullptr)
		{
			shell->input_refMat = data.getRefMat(0);
		}
		if (shell->input_affineTrans == nullptr)
		{
			shell->input_affineTrans = data.getAffineTrans(0);
		}
		if (isWidgetShow)
		{
			FindLineWidget* shellWidget = (FindLineWidget*)(OpratorWidget->at(index));
			shellWidget->setWindowTitle(title);
			shellWidget->getImageComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_mat);
			shellWidget->getTransComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_affineTrans);
			shellWidget->setOperator(shell);
			shellWidget->hide();
			shellWidget->raise();
			shellWidget->show();
			return;
		}
	}
	if (baseTool->GetNodeName() == "findCircle")
	{
		FindCircleOperator* shell = (FindCircleOperator*)baseTool;
		InputData data;
		data.append(loadOperators(index, ToolDataType::eMat));
		data.append(loadOperators(index, ToolDataType::eRefMat));
		data.append(loadOperators(index, ToolDataType::eAffineTrans));
		if (shell->input_mat == nullptr)
		{
			shell->input_mat = data.getMat(0);
		}
		if (shell->input_refMat == nullptr)
		{
			shell->input_refMat = data.getRefMat(0);
		}
		if (shell->input_affineTrans == nullptr)
		{
			shell->input_affineTrans = data.getAffineTrans(0);
		}
		if (isWidgetShow)
		{
			FindCircleWidget* shellWidget = (FindCircleWidget*)(OpratorWidget->at(index));
			shellWidget->setWindowTitle(title);
			shellWidget->getImageComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_mat);
			shellWidget->getTransComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_affineTrans);
			shellWidget->setOperator(shell);
			shellWidget->hide();
			shellWidget->raise();
			shellWidget->show();
			return;
		}
	}
	if (baseTool->GetNodeName() == "shapeMatch")
	{
		ShapeMatchOperator* shell = (ShapeMatchOperator*)baseTool;
		InputData data;
		data.append(loadOperators(index, ToolDataType::eMat));
		data.append(loadOperators(index, ToolDataType::eRefMat));
		data.append(loadOperators(index, ToolDataType::eAffineTrans));
		if (shell->input_mat == nullptr)
		{
			shell->input_mat = data.getMat(0);
		}
		if (shell->input_refMat == nullptr)
		{
			shell->input_refMat = data.getRefMat(0);
		}
		if (isWidgetShow)
		{
			ShapeMatchWidget* shellWidget = (ShapeMatchWidget*)(OpratorWidget->at(index));
			shellWidget->setWindowTitle(title);
			shellWidget->getImageComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_mat);
			shellWidget->setOperator(shell);
			shellWidget->hide();
			shellWidget->raise();
			shellWidget->show();
			return;
		}
	}
	if (baseTool->GetNodeName() == "enhancement")
	{
		EnhancementOperator* shell = (EnhancementOperator*)baseTool;
		InputData data;
		data.append(loadOperators(index, ToolDataType::eMat));
		data.append(loadOperators(index, ToolDataType::eRefMat));
		if (shell->input_mat == nullptr)
		{
			shell->input_mat = data.getMat(0);
		}
		if (shell->input_refMat == nullptr)
		{
			shell->input_refMat = data.getRefMat(0);
		}
		if (isWidgetShow)
		{
			EnhancementWidget* shellWidget = (EnhancementWidget*)(OpratorWidget->at(index));
			shellWidget->setWindowTitle(title);
			shellWidget->getImageComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_mat);
			shellWidget->setOperator(shell);
			shellWidget->hide();
			shellWidget->raise();
			shellWidget->show();
			return;
		}
	}
	if (baseTool->GetNodeName() == "morphology")
	{
		MorphologyOperator* shell = (MorphologyOperator*)baseTool;
		InputData data;
		data.append(loadOperators(index, ToolDataType::eMat));
		if (shell->input_mat == nullptr)
		{
			shell->input_mat = data.getMat(0);
		}
		if (shell->input_refMat == nullptr)
		{
			shell->input_refMat = data.getRefMat(0);
		}
		if (isWidgetShow)
		{
			MorphologyWidget* shellWidget = (MorphologyWidget*)(OpratorWidget->at(index));
			shellWidget->setWindowTitle(title);
			shellWidget->getImageComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_mat);
			shellWidget->setOperator(shell);
			shellWidget->hide();
			shellWidget->raise();
			shellWidget->show();
			return;
		}
	}
	if (baseTool->GetNodeName() == "smooth")
	{
		SmoothOperator* shell = (SmoothOperator*)baseTool;
		InputData data;
		data.append(loadOperators(index, ToolDataType::eMat));
		if (shell->input_mat == nullptr)
		{
			shell->input_mat = data.getMat(0);
		}
		if (shell->input_refMat == nullptr)
		{
			shell->input_refMat = data.getRefMat(0);
		}
		if (isWidgetShow)
		{
			SmoothWidget* shellWidget = (SmoothWidget*)(OpratorWidget->at(index));
			shellWidget->setWindowTitle(title);
			shellWidget->getImageComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_mat);
			shellWidget->setOperator(shell);
			shellWidget->hide();
			shellWidget->raise();
			shellWidget->show();
			return;
		}
	}
	if (baseTool->GetNodeName() == "threshold")
	{
		ThresholdOperator* shell = (ThresholdOperator*)baseTool;
		InputData data;
		data.append(loadOperators(index, ToolDataType::eMat));
		if (shell->input_mat == nullptr)
		{
			shell->input_mat = data.getMat(0);
		}
		if (shell->input_refMat == nullptr)
		{
			shell->input_refMat = data.getRefMat(0);
		}
		if (isWidgetShow)
		{
			ThresholdWidget* shellWidget = (ThresholdWidget*)(OpratorWidget->at(index));
			shellWidget->setWindowTitle(title);
			shellWidget->getImageComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_mat);
			shellWidget->setOperator(shell);
			shellWidget->hide();
			shellWidget->raise();
			shellWidget->show();
			return;
		}
	}

	if (baseTool->GetNodeName() == "Barcode")
	{
		BarcodeOperator* shell = (BarcodeOperator*)baseTool;
		InputData data;
		data.append(loadOperators(index, ToolDataType::eMat));
		if (shell->input_mat == nullptr)
		{
			shell->input_mat = data.getMat(0);
		}
		if (shell->input_refMat == nullptr)
		{
			shell->input_refMat = data.getRefMat(0);
		}
		if (isWidgetShow)
		{
			BarcodeWidget* shellWidget = (BarcodeWidget*)(OpratorWidget->at(index));
			shellWidget->setWindowTitle(title);
			shellWidget->getImageComboBox()->setInputData(data)->loadItems()->selectItem(shell->input_mat);
			shellWidget->setOperator(shell);
			shellWidget->hide();
			shellWidget->raise();
			shellWidget->show();
			return;
		}
	}
}
void ToolBlockManager::initDisplay(bool isWidgetShow)
{
	//if (m_inspection == nullptr)
	// return;
	//DisplayShell* shell = (DisplayShell*)m_inspection->m_shellTreeData->getDisplaySetting();
	//InputData data;
	//int index = m_toolBlock->Children().size();
	//data.append(loadShells(index, ToolDataType::eMat));
	//if (shell->input_mat == nullptr)
	//{
	// shell->input_mat = data.getMat(0);
	//}
	//if (isWidgetShow)
	//{
	// QString title = QString::fromStdString(m_inspection->GetName()) + " -> Display";
	// DisplayWidget* shellWidget = new DisplayWidget();
	// shellWidget->setWindowTitle(title);
	// shellWidget->inputBox_srcImage->setInputData(data)
	// ->loadItems(index)->selectItem(shell->input_mat);
	// shellWidget->setShell(shell);
	// shellWidget->doConnect();
	// shellWidget->updateDisplayTree();
	// shellWidget->show();
	//}
}
void ToolBlockManager::initClientResultEdiWidget()
{
}
void ToolBlockManager::initOperatorsOutput(ToolBlock * toolBlock)
{
	for each (auto node in toolBlock->Children())
	{
		BaseOperator* tool = (BaseOperator*)node;
		tool->registerOutput();
		tool = nullptr;
	}
}
void ToolBlockManager::initOperatorInput(ToolBlock * toolBlock)
{
	for each(auto var in toolBlock->Children())
	{
		if (var->GetNodeName() == "shapeMatch")
		{
			ShapeMatchOperator *opera = (ShapeMatchOperator*)var;
			opera->loadTemplate();
			opera->createTemplate();
		}
	}

}
void ToolBlockManager::initDisplayOperator(ToolBlock * toolBlock, BaseNode * displayShell)
{
}