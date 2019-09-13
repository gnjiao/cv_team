#pragma once

#include <vector>

#include "DataModel\ToolDataMap.h"


/*
******这个类是输入的数据，跟输出结果差不多，描述的是每个算子的输入数据，有图像/点/线/圆等数据
******
******
******
******
******
******
*/


using namespace toolModel;

using namespace std;

using namespace geo;



class InputData
{
public:

	InputData();

	~InputData();



	InputData append(InputData data);



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



	vector<ToolData<int>*> vectorInt;

	vector<ToolData<double>*> vectorDouble;

	vector<ToolData<bool>*> vectorBool;

	vector<ToolData<string>*> vectorString;

	vector<ToolData<cv::Mat>*> vectorMat;

	vector<ToolData<Point2D>*> vectorPoint;

	vector<ToolData<Line2D>*> vectorLine;

	vector<ToolData<Circle2D>*> vectorCircle;

	vector<ToolData<double>*> vectorLength;

	vector<ToolData<double>*> vectorArea;

	vector<ToolData<Angle>*> vectorAngle;

	vector<ToolData<double>*> vectorScore;

	vector<ToolData<AffineTrans>*> vectorAffineTrans;

	vector<ToolData<cv::Mat>*> vectorRefMat;

};


