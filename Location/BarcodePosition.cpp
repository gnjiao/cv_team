#include "BarcodePosition.h"

BarcodePosition::BarcodePosition()
{

}

BarcodePosition::~BarcodePosition()
{
}

vector<Point> BarcodePosition::FindBarcode(Mat src)
{
	Mat src_gray;
	cvtColor(src, src_gray, CV_BGR2GRAY);
	//对图像进行平滑处理  
	blur(src_gray, src_gray, Size(3, 3));
	//使灰度图象直方图均衡化  
	equalizeHist(src_gray, src_gray);
	//阈值化
	Mat threshold_output;
	threshold(src_gray, threshold_output, 112, 255, THRESH_BINARY); //指定112阀值进行二值化

	Scalar color = Scalar(1, 1, 255);
	vector<vector<Point>> contours, contours2;
	vector<Vec4i> hierarchy;
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	//Mat drawing2 = Mat::zeros( src.size(), CV_8UC3 );  
	Mat drawingAllContours = Mat::zeros(src.size(), CV_8UC3);

	//利用二值化输出寻找轮廓
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

	//寻找轮廓的方法
	int tempindex1 = 0;
	int tempindex2 = 0;

	for (int i = 0;i<contours.size();i++)
	{
		if (hierarchy[i][2] == -1)
			continue;
		else
			tempindex1 = hierarchy[i][2];                //第一个子轮廓的索引

		if (hierarchy[tempindex1][2] == -1)
			continue;
		else
		{
			tempindex2 = hierarchy[tempindex1][2];        //第二个子轮廓的索引
														  //记录搜索到的有两个子轮廓的轮廓并把他们的编号存储
			in.a1 = i;
			in.a2 = tempindex1;
			in.a3 = tempindex2;
			vin.push_back(in);
		}
	}
	//按面积比例搜索
	vector<index>::iterator it;
	for (it = vin.begin();it != vin.end();)
	{
		vector<Point> out1Contours = contours[it->a1];
		vector<Point> out2Contours = contours[it->a2];
		double lenth1 = arcLength(out1Contours, 1);
		double lenth2 = arcLength(out2Contours, 1);
		if (abs(lenth1 / lenth2 - 2)>1)
		{
			it = vin.erase(it);
		}
		else
		{
			drawContours(drawing, contours, it->a1, Scalar(255, 255, 255), CV_FILLED, 8); //rng.uniform(0,255)
			it++;
		}
	}
	//获取三个定位角的中心坐标  
	Point point[3];
	int i = 0;
	vector<Point> pointThree;
	for (it = vin.begin(), i = 0;it != vin.end();i++, it++)
	{
		point[i] = Center_cal(contours, it->a1);
		pointThree.push_back(point[i]);
	}

	if (pointThree.size() <3)
	{
		cout << "找到的定位角点不足3个" << endl;
		return vector<Point>();
	}
	//计算轮廓的面积，计算定位角的面积，从而计算出边长
	double area = contourArea(contours[vin[0].a1]);
	int area_side = cvRound(sqrt(double(area)));
	for (int i = 0; i<3; i++)
	{
		//画出三个定位角的中心连线  
		line(drawing, point[i % 3], point[(i + 1) % 3], color, area_side / 10, 8);
	}
	vin.clear();
	return pointThree;
}

Point BarcodePosition::Center_cal(vector<vector<Point>> contours, int i)
{
	int centerx = 0, centery = 0;
	size_t n = contours[i].size();
	//在提取的小正方形的边界上每隔周长个像素提取一个点的坐标，  
	//求所提取四个点的平均坐标（即为小正方形的大致中心）  
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	Point point1 = Point(centerx, centery);
	return point1;
}
