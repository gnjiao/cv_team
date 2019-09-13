#pragma once
#include "DataModel\BaseWidget.h"
#include "AcquireImageOperator.h"
#include "GraphicsItem\FrontSightItem.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDoublespinBox>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
class AcquireImageWidget :public BaseWidget
{
	Q_OBJECT
public:
	AcquireImageWidget();
	~AcquireImageWidget();
	void setOperator(AcquireImageOperator *opra);
private slots:
	void on_Register_btn_Clicked();
private:
	void doOpenFile(cv::Mat img);
	void doEdit(bool b);
	void doEditSave();
	void doEditCancel();
	void doPlay();
	void doOk();
	void doCancel();
	void makeBaseWidget();
	void initWidget();
	void showImage();
	AcquireImageOperator* m_operator;
	cv::Mat m_Mat;
	cv::Mat m_refMat;
	Inspection *m_inspection;
	QPushButton * m_Register_btn;
	QCheckBox* m_OnOff_cb = nullptr;
	QComboBox* m_Type_cb = nullptr;
	QPushButton * m_FrontSightColor_btn;
	QDoubleSpinBox* m_CenterSize_sp = nullptr;
	QLabel* m_Ratio_lb = nullptr;
	FrontSightItem* m_sight = nullptr;
};