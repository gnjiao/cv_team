#include "OperatorManager.h"

OperatorManager::OperatorManager()
{
}
OperatorManager::~OperatorManager()
{
}
BaseOperator * OperatorManager::createOperator(string nodeName)
{
	BaseOperator* newOperator = nullptr;
	if (nodeName == "findLine")
	{
		newOperator = new FindLineOperator();
	}
	else if (nodeName == "findCircle")
	{
		newOperator = new FindCircleOperator();
	}
	else if (nodeName == "shapeMatch")
	{
		newOperator = new ShapeMatchOperator();
	}
	else if (nodeName == "enhancement")
	{
		newOperator = new EnhancementOperator();
	}
	else if (nodeName == "morphology")
	{
		newOperator = new MorphologyOperator();
	}
	else if (nodeName == "smooth")
	{
		newOperator = new SmoothOperator();
	}
	else if (nodeName == "threshold")
	{
		newOperator = new ThresholdOperator();
	}
	else if (nodeName == "Barcode")
	{
		newOperator = new BarcodeOperator();
	}

	return newOperator;
}