#include <iostream> 
#include <string>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>
#include <vector>
using namespace std;
using namespace cv;

RNG rng(12345);

struct index
{
	int a1;
	int a2;
	int a3;
};

class BarcodePosition
{
public:
	BarcodePosition();
	~BarcodePosition();
	vector<Point> FindBarcode(Mat src);
private:
	Point Center_cal(vector<vector<Point> > contours, int i);

private:
	index in;
	vector<index> vin;
};