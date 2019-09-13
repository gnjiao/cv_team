#pragma once
#include "DataModel\BaseWidget.h"
#include "DataModel\InputBox.h"
#include "ThresholdOperator.h"
#include <QGroupBox>
class ThresholdWidget : public BaseWidget
{
	Q_OBJECT
public:
	ThresholdWidget(QWidget *parent = Q_NULLPTR);
	~ThresholdWidget();

	InputBox *getImageComboBox();
	void setOperator(ThresholdOperator* opra);
	void doPlay();
	void doOk();
	void doCancel();
private slots:
	void on_image_cb_Changed(int index);

private:
	void syncOperator(ThresholdOperator* AA, ThresholdOperator* BB);
	void showImage();
	void makeWidget();
	InputBox* Image_cb;
	QComboBox* Mode_cb;
	ThresholdOperator* SaveOperator = nullptr;
	ThresholdOperator* CalOperator = nullptr;
};