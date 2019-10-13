#pragma once
#include "DataModel\BaseWidget.h"
#include "DataModel\InputBox.h"
#include "BarcodeOperator.h"
#include "GraphicsItem\TextItem.h"
#include <QGroupBox>

class BarcodeWidget : public BaseWidget
{
	Q_OBJECT
public:
	BarcodeWidget(QWidget *parent = Q_NULLPTR);
	~BarcodeWidget();
	InputBox *getImageComboBox();
	void setOperator(BarcodeOperator* opra);
	void doPlay();
	void doOk();
	void doCancel();
	void doEdit(bool checked);
	private slots:
	void on_image_cb_Changed(int index);
	void on_Mode_cb_Changed(int index);
private:
	void syncOperator(BarcodeOperator* AA, BarcodeOperator* BB);
	void showImage();
	void makeWidget();
	void updateOutput();
	InputBox* Image_cb;
	QComboBox* Mode_cb;
	QLabel *Result_lb;
	BarcodeOperator* SaveOperator = nullptr;
	BarcodeOperator* CalOperator = nullptr;
	TextItem *resultItem;
};