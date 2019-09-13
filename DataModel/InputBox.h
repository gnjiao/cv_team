#pragma once

#include <QComboBox>

#include "DataModel\InputData.h"


class InputBox : public QComboBox

{

	Q_OBJECT



public:

	InputBox(QWidget *parent = Q_NULLPTR);

	~InputBox();



	InputBox* setInputData(InputData data);

	InputBox* setDataType(ToolDataType dataType);

	InputBox* loadItems();



	void selectItem(ToolData<int>* toolData);

	void selectItem(ToolData<double>* toolData);

	void selectItem(ToolData<bool>* toolData);

	void selectItem(ToolData<string>* toolData);

	void selectItem(ToolData<cv::Mat>* toolData);

	void selectItem(ToolData<Point2D>* toolData);

	void selectItem(ToolData<Line2D>* toolData);

	void selectItem(ToolData<Circle2D>* toolData);

	void selectItem(ToolData<Angle>* toolData);

	void selectItem(ToolData<AffineTrans>* toolData);



	ToolData<int>* getInt(int index);

	ToolData<double>* getDouble(int index);

	ToolData<bool>* getBool(int index);

	ToolData<string>* getString(int index);

	ToolData<cv::Mat>* getMat(int index);

	ToolData<Point2D>* getPoint(int index);

	ToolData<Line2D>* getLine(int index);

	ToolData<Circle2D>* getCircle(int index);

	ToolData<double>* getLength(int index);

	ToolData<double>* getArea(int index);

	ToolData<Angle>* getAngle(int index);

	ToolData<double>* getScore(int index);

	ToolData<AffineTrans>* getAffineTrans(int index);

	ToolData<cv::Mat>* getRefMat(int index);



private:

	ToolDataType m_dataType;

	InputData m_inputData;

};

