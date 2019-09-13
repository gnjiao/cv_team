#pragma once
#include "DataModel\BaseWidget.h"
#include "DataModel\InputBox.h"
#include "SmoothOperator.h"
#include <QGroupBox>
class SmoothWidget : public BaseWidget
{
	Q_OBJECT
public:
	SmoothWidget(QWidget *parent = Q_NULLPTR);
	~SmoothWidget();

	InputBox *getImageComboBox();
	void setOperator(SmoothOperator* opra);
	void doPlay();
	void doOk();
	void doCancel();
private slots:
	void on_image_cb_Changed(int index);

private:
	void syncOperator(SmoothOperator* AA, SmoothOperator* BB);
	void showImage();
	void makeWidget();
	InputBox* Image_cb;
	QComboBox* Mode_cb;
	SmoothOperator* SaveOperator = nullptr;
	SmoothOperator* CalOperator = nullptr;
};