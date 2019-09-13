#pragma once
#include "DataModel\BaseWidget.h"
#include "DataModel\InputBox.h"
#include "EnhancementOperator.h"
#include <QGroupBox>
class EnhancementWidget : public BaseWidget
{
	Q_OBJECT
public:
	EnhancementWidget(QWidget *parent = Q_NULLPTR);
	~EnhancementWidget();
	
	InputBox *getImageComboBox();
	void setOperator(EnhancementOperator* opra);
	void doPlay();
	void doOk();
	void doCancel();
private slots:
	void on_image_cb_Changed(int index);
	void on_Mode_cb_Changed(int index);

private:
	void syncOperator(EnhancementOperator* AA, EnhancementOperator* BB);
	void showImage();
	void makeWidget();
	InputBox* Image_cb;
	QComboBox* Mode_cb;
	EnhancementOperator* SaveOperator = nullptr;
	EnhancementOperator* CalOperator = nullptr;
};