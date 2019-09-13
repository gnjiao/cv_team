#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <Eigen\Dense>
#include <Eigen\LU>
#include "zbar.h"
using namespace std;
using namespace zbar;

class Barcode
{
public:
	Barcode();
	~Barcode();
	int DecodeQR(cv::Mat src, string &type, string &result);
private:
	int ZbarDecoder(cv::Mat img, string &type, string &result);
	int GetQRInBinImg(cv::Mat binImg, string &type, string &result);
	int GetQR(cv::Mat img, string &type, string &result);
};