#pragma once
#include <QtWidgets/QWidget>
#include "LTwindow\LTwindow.h"
#include "LTwindow\LTBar.h"
#include "LTwindow\LTButton.h"
#include "LTwindow\quickAccessBar.h"
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QSpinBox>
#include "DataModel\FrameView.h"
class CameraWidget : public LTwindow
{
	Q_OBJECT
public:
	CameraWidget(QWidget *parent = Q_NULLPTR);
	void createMenuBar();
	void createQuickAccessBar();
	void createImageView();
	void createCentralWidget();
private:
	FrameView *currentView;
	FrameView *captureView;
	QHBoxLayout *hlay_main;
	QPushButton *searchDevice_btn;
	QPushButton *open_btn;
	QPushButton *close_btn;
	QRadioButton *continuousMode_rbtn;
	QRadioButton *triggerMode_rbtn;
	QPushButton *startGrab_btn;
	QPushButton *stopGrab_btn;
	QCheckBox *softTrigger_cb;
	QPushButton *softTrigger_btn;
	QPushButton *savePng_btn;
	QPushButton *saveBmp_btn;
	QPushButton *saveJpg_btn;
	QSpinBox *exposureTime_sp;
	QSpinBox *frameRate_sp;
	QSpinBox *gain_sp;
	QPushButton *getParam_btn;
	QPushButton *setParam_btn;
};