#pragma once
#include "DataModel\InputData.h"
#include "Location\FindLineWidget.h"
#include "Acquisition\AcquireImageWidget.h"
#include "Location\FindCircleWidget.h"
#include "Location\ShapeMatchWidget.h"
#include "PreTreatment\EnhancementWidget.h"
#include "PreTreatment\MorphologyWidget.h"
#include "PreTreatment\SmoothWidget.h"
#include "PreTreatment\ThresholdWidget.h"
#include "Recognition\BarcodeWidget.h"
#include "ToolBlock.h"
#define ToolBlockMGR ToolBlockManager::Instance()
class ToolBlockManager
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_toolKit;
		//ar & m_displaySetting;
		//ar & m_outputSetting;
	}
public:
	ToolBlockManager();
	~ToolBlockManager();
	static ToolBlockManager* Instance()
	{
		static ToolBlockManager instance;
		return &instance;
	}
	void setInspection(Inspection* inspection);
	Inspection* getInspection()const;
	ToolBlock* getToolBlock()const;
	void save();
	ToolBlock* load(string m_fileName);
	InputData loadOperators(int index, ToolDataType dataType);
	void initOperator(int index, bool isWidgetShow, vector<QDialog*> *mshellwidgets = nullptr);
	void initDisplay(bool isWidgetShow);
	void initClientResultEdiWidget();
	void initOperatorsOutput(ToolBlock *toolBlock);
	void initOperatorInput(ToolBlock *toolBlock);
	void initDisplayOperator(ToolBlock *toolBlock, BaseNode *displayShell);
private:
	Inspection* m_inspection = nullptr;
	ToolBlock* m_toolBlock = nullptr;
};

