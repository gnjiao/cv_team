#pragma once
#include "DataModel\BaseOperator.h"
#include "ToolBlock.h"
#include "Location\findLineOperator.h"
#include "Location\FindCircleOperator.h"
#include "Location\ShapeMatchOperator.h"
#include "PreTreatment\EnhancementOperator.h"
#include "PreTreatment\MorphologyOperator.h"
#include "PreTreatment\SmoothOperator.h"
#include "PreTreatment\ThresholdOperator.h"

#define OperatorMGR OperatorManager::Instance()
using namespace std;
using namespace toolModel;
class OperatorManager
{
public:
	OperatorManager();
	~OperatorManager();
	static OperatorManager* Instance()
	{
		static OperatorManager instance;
		return &instance;
	}
	BaseOperator* createOperator(string nodeName);
};