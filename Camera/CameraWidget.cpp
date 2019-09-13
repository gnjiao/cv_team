#include "CameraWidget.h"
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
CameraWidget::CameraWidget(QWidget *parent)
	: LTwindow(parent)
{
	this->setObjectName("InspectionDialog");
	this->setWindowTitle("Camera Widget");
	this->resize(1100, 750);
	setWindowIcon(QIcon(":/main/logo"));
	//createMenuBar();//
	createQuickAccessBar();
	createCentralWidget();
}
void CameraWidget::createMenuBar()
{
	if (QMenuBar* menuBar = LtBar()->menuBar())
	{
		QAction *action = new QAction(tr("action1"), this);
		QMenu* menu = menuBar->addMenu(tr("&File"));
		menu->addAction(action);
		action = new QAction(tr("action1"), this);
		menu = menuBar->addMenu(tr("&Edit"));
		menu->addAction(action);
		action = new QAction(tr("action1"), this);
		menu = menuBar->addMenu(tr("&Debug"));
		menu->addAction(action);
		action = new QAction(tr("action1"), this);
		menu = menuBar->addMenu(tr("&View"));
		menu->addAction(action);
		action = new QAction(tr("action1"), this);
		menu = menuBar->addMenu(tr("&Window"));
		menu->addAction(action);
		action = new QAction(tr("action1"), this);
		menu = menuBar->addMenu(tr("&Tools"));
		menu->addAction(action);
		action = new QAction(tr("action1"), this);
		menu = menuBar->addMenu(tr("&Help"));
		menu->addAction(action);
		LtBar()->showMenuBar(true);
	}
	QTextBrowser *text = new QTextBrowser(this);
	LtBar()->setApplicationWidget(tr("Application"), text);
	LtBar()->setLTStyle(LTBar::WindowStyle);
	LTButton *m_Button = new LTButton();
	m_Button->setHasMenu(true);
	m_Button->setText(tr("MebuButton"));
	m_Button->setToolButtonStyle(Qt::ToolButtonTextOnly);
	m_Button->setNormalColor(QColor(240, 130, 0));
	LtBar()->menuLayout()->addWidget(m_Button);
}
void CameraWidget::createQuickAccessBar()
{
	if (QuickAccessBar* quickAccessBar = LtBar()->quickAccessBar())
	{
		QAction* action = quickAccessBar->actionCustomizeButton();
		action->setToolTip(tr("Customize Quick Access Bar"));
		QAction* smallButton = quickAccessBar->addAction(QIcon(":/tools/new"), tr("New"));
		smallButton->setShortcut(QKeySequence::New);
		smallButton->setToolTip(tr("New File or Project\nCtrl+N"));
		connect(smallButton, SIGNAL(triggered()), this, SLOT(slotNew()));
		// quickAccessBar->setActionVisible(smallButton, false);
		smallButton = quickAccessBar->addAction(QIcon(":/tools/open"), tr("Open"));
		smallButton->setShortcut(tr("Ctrl+O"));
		smallButton->setToolTip(tr("Open File or Project\nCtrl+O"));
		// connect(smallButton, SIGNAL(triggered()), this, SLOT(slotOpen()));
		// quickAccessBar->setActionVisible(smallButton, false);
		QMenu *menu = new QMenu(this);
		menu->addAction(tr("action1"));
		menu->addAction(tr("action2"));
		menu->addAction(tr("action3"));
		menu->addAction(tr("action4"));
		smallButton->setMenu(menu);
		smallButton = quickAccessBar->addAction(QIcon(":/tools/save"), tr("Save"));
		smallButton->setShortcut(tr("Ctrl+S"));
		smallButton->setToolTip(tr("Save the active document\nCtrl+S"));
		connect(smallButton, SIGNAL(triggered()), this, SLOT(slotSave()));
		smallButton = quickAccessBar->addAction(QIcon(":/tools/save_all"), tr("Save All"));
		smallButton->setShortcut(tr("Ctrl+Shift+S"));
		smallButton->setToolTip(tr("Save all active documents\nCtrl+Shift+S"));
		// connect(smallButton, SIGNAL(triggered()), this, SLOT(pressButton()));
		smallButton = quickAccessBar->addAction(QIcon(":/tools/undo"), tr("&Undo"));
		smallButton->setShortcut(QKeySequence::Undo);
		smallButton->setEnabled(false);
		smallButton = quickAccessBar->addAction(QIcon(":/tools/redo"), tr("&Redo"));
		smallButton->setShortcut(QKeySequence::Redo);
		// smallButton->setEnabled(false);
		LtBar()->showQuickAccess(true);
		LtBar()->setBackgroundColor(QColor(115, 115, 115, 70));
	}
}
void CameraWidget::createImageView()
{
	currentView = new FrameView();
	captureView = new FrameView();
	//CurrentView->setFixedSize(QSize(600, 600));
	//CaptureView->setFixedSize(QSize(600, 600));
	QTabWidget *viewWidget = new QTabWidget();
	viewWidget->setContentsMargins(0, 0, 0, 0);
	viewWidget->addTab(currentView, QString::fromStdWString(L"ÊµÊ±Í¼Ïñ"));
	viewWidget->addTab(captureView, QString::fromStdWString(L"²¶»ñÍ¼Ïñ"));
	hlay_main = new QHBoxLayout();
	hlay_main->setContentsMargins(0, 0, 0, 0);
	hlay_main->addWidget(viewWidget);
	//this->setCentralWidget(viewWidget);
}
void CameraWidget::createCentralWidget()
{
	createImageView();
	searchDevice_btn = new QPushButton("Search Device");
	open_btn = new QPushButton("Open Camera");
	close_btn = new QPushButton("Close Camera");
	continuousMode_rbtn = new QRadioButton("Continuous Mode");
	triggerMode_rbtn = new QRadioButton("Trigger Mode");
	startGrab_btn = new QPushButton("Start Grab");
	stopGrab_btn = new QPushButton("Stop Grab");
	softTrigger_cb = new QCheckBox("Soft Trigger");
	softTrigger_btn = new QPushButton("Soft Trigger");
	savePng_btn = new QPushButton("Save png");
	saveBmp_btn = new QPushButton("Save bmp");
	saveJpg_btn = new QPushButton("Save jpg");
	exposureTime_sp = new QSpinBox();
	frameRate_sp = new QSpinBox();
	gain_sp = new QSpinBox();
	getParam_btn = new QPushButton("Get Parameter");
	setParam_btn = new QPushButton("Set Parameter");
	QVBoxLayout *vlay_right = new QVBoxLayout();
	QGroupBox *init_gb = new QGroupBox("Init");
	QVBoxLayout*vlay1 = new QVBoxLayout();
	QHBoxLayout *hlay1 = new QHBoxLayout();
	hlay1->addWidget(open_btn);
	hlay1->addWidget(close_btn);
	vlay1->addWidget(searchDevice_btn);
	vlay1->addLayout(hlay1);
	init_gb->setLayout(vlay1);
	QGroupBox *imageGrab_gb = new QGroupBox("Grab Image");
	QVBoxLayout*vlay2 = new QVBoxLayout();
	QGridLayout *glay1 = new QGridLayout();
	glay1->addWidget(continuousMode_rbtn, 0, 0);
	glay1->addWidget(triggerMode_rbtn, 0, 1);
	glay1->addWidget(startGrab_btn, 1, 0);
	glay1->addWidget(stopGrab_btn, 1, 1);
	glay1->addWidget(softTrigger_cb, 2, 0);
	glay1->addWidget(softTrigger_btn, 2, 1);
	imageGrab_gb->setLayout(glay1);
	QGroupBox *saveImage_gb = new QGroupBox("Save Image");
	QHBoxLayout *hlay2 = new QHBoxLayout();
	hlay2->addWidget(saveBmp_btn);
	hlay2->addWidget(savePng_btn);
	saveImage_gb->setLayout(hlay2);
	QGroupBox *param_gb = new QGroupBox("Parameter");
	QVBoxLayout*vlay3 = new QVBoxLayout();
	QHBoxLayout *hlay3 = new QHBoxLayout();
	hlay3->addWidget(new QLabel("Exposure Time"));
	hlay3->addWidget(exposureTime_sp);
	QHBoxLayout *hlay4 = new QHBoxLayout();
	hlay4->addWidget(new QLabel("Gain"));
	hlay4->addWidget(gain_sp);
	QHBoxLayout *hlay5 = new QHBoxLayout();
	hlay5->addWidget(new QLabel("Frame Rate"));
	hlay5->addWidget(frameRate_sp);
	QHBoxLayout *hlay6 = new QHBoxLayout();
	hlay6->addWidget(setParam_btn);
	hlay6->addWidget(getParam_btn);
	vlay3->addLayout(hlay3);
	vlay3->addLayout(hlay4);
	vlay3->addLayout(hlay5);
	vlay3->addLayout(hlay6);
	param_gb->setLayout(vlay3);
	hlay_main->addLayout(vlay_right);
	vlay_right->addWidget(init_gb);
	vlay_right->addWidget(imageGrab_gb);
	vlay_right->addWidget(saveImage_gb);
	vlay_right->addWidget(param_gb);
	hlay_main->setContentsMargins(20, 20, 20, 0);
	QWidget *w = new QWidget();
	w->setLayout(hlay_main);
	this->setCentralWidget(w);
}