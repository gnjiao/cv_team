#include "recognition\Barcode.h"
Barcode::Barcode()
{
}
Barcode::~Barcode()
{
}



//½âÎö¶þÎ¬Âë
int Barcode::ZbarDecoder(cv::Mat img, string &type, string &result)
{
	ImageScanner scanner;
	const void *raw = (&img)->data;
	// configure the reader
	scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
	// wrap image data
	Image image(img.cols, img.rows, "Y800", raw, img.cols * img.rows);
	// scan the image for bar codes
	int n = scanner.scan(image);
	// extract results
	Image::SymbolIterator symbol = image.symbol_begin();
	if (image.symbol_begin() == image.symbol_end())
	{
		cout << "Check Image Please" << endl;
	}
	for (; symbol != image.symbol_end(); ++symbol)
	{
		//cout << "Code Type:" << endl << symbol->get_type_name() << endl << endl;
		//cout << "Code Data:" << endl << symbol->get_data() << endl << endl;
		type = symbol->get_type_name();
		result = symbol->get_data();
	}
	image.set_data(NULL, 0);
	return 0;
}
int Barcode::GetQRInBinImg(cv::Mat binImg, string &type, string &result)
{
	int ret = ZbarDecoder(binImg, type, result);
	if (result.empty())
	{
		cv::Mat openImg;
		cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		morphologyEx(binImg, openImg, cv::MORPH_OPEN, element);
		int ret = ZbarDecoder(openImg, type, result);
	}
	return ret;
}
int Barcode::GetQR(cv::Mat img, string &type, string &result)
{
	cv::Mat binImg;
	//
	double thre = threshold(img, binImg, 0, 255, cv::THRESH_OTSU);
	while (result.empty() && thre < 255)
	{
		threshold(img, binImg, thre, 255, cv::THRESH_BINARY);
		int ret = GetQRInBinImg(binImg, type, result);
		thre += 20;//
	}
	return 0;
}
int Barcode::DecodeQR(cv::Mat src, string &type, string &result)
{
	if (!src.data)
	{
		cout << "image null" << endl;
		return 1;
	}
	int ret = GetQR(src, type, result);
	if (result.size() == 0)
	{
		return 2;
	}
	else
	{
		return ret;
	}
}