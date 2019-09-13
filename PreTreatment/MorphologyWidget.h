#pragma once
#include "DataModel\BaseWidget.h"
#include "DataModel\InputBox.h"
#include "MorphologyOperator.h"

#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QIntValidator>
#include <QColor>
#include <QColorDialog>
using namespace geo;
class MorphologyWidget : public BaseWidget
{
	Q_OBJECT
public:
	MorphologyWidget(QWidget *parent = Q_NULLPTR);
	~MorphologyWidget();
	void doPlay();
	void doOk();
	void doCancel();
	void doEdit(bool b);
	void doEditSave();
	void setOperator(MorphologyOperator *Opra);
	void updateOutput();
	InputBox *getImageComboBox();

private slots:
	void on_MorphologyMode_cob_Changed(int);
	void on_KernelShape_cob_Changed(int);
	void on_KernelSize_sl_Changed(int);
	void on_KernelSize_sp_Changed(int);
	void on_IterCount_sl_Changed(int);
	void on_IterCount_sp_Changed(int);
private:
	void makeWidget();
	void initWidget();
	void syncOperator(MorphologyOperator* AA, MorphologyOperator* BB);
	void showImage();

	InputBox *image_cb;
	QComboBox *MorphologyMode_cob;
	QComboBox *KernelShape_cob;
	QSlider *KernelSize_sl;
	QSpinBox *KernelSize_sp;
	QSlider *IterCount_sl;
	QSpinBox *IterCount_sp;

private:
	MorphologyOperator *CalOperator;
	MorphologyOperator *SaveOperator;


};