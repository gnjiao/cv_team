#include "CV_TEAM.h"
#include "Pretreatment\MorphologyWidget.h"
#include "Pretreatment\EnhancementWidget.h"
CV_TEAM::CV_TEAM(QWidget *parent)
	: QMainWindow(parent)
{

	this->setWindowFlag(Qt::FramelessWindowHint);
	this->setWindowTitle("Computer Vision Team");
	this->setWindowIcon(QIcon(":/CV_TEAM/icons/mainWindow.png"));
	QLabel *label = new QLabel(this);
	label->setPixmap(QPixmap(":/CV_TEAM/icons/logo.jpg"));
	label->setGeometry(1700, 20, 155, 60);
	makeMenu();
	makeCenturalWidget();
	makeDockWidget();
	this->showMaximized();

}

void CV_TEAM::makeMenu()
{
	MainAPP_mn = this->menuBar()->addMenu("Main(&M)");
	MainAPP_mn->setShortcutEnabled(true);
	Start_ac = new QAction("Start");
	Stop_ac = new QAction("Stop");
	Exit_ac = new QAction("Exit");
	MainAPP_mn->addAction(Start_ac);
	MainAPP_mn->addAction(Stop_ac);
	MainAPP_mn->addAction(Exit_ac);
	Start_ac->setIcon(QIcon(":/CV_TEAM/icons/start.png"));
	Stop_ac->setIcon(QIcon(":/CV_TEAM/icons/stop.png"));
	Exit_ac->setIcon(QIcon(":/CV_TEAM/icons/exit.png"));
	Start_ac->setShortcut(QKeySequence(tr("Ctrl+S")));
	Stop_ac->setShortcut(QKeySequence(tr("Ctrl+D")));
	Exit_ac->setShortcut(QKeySequence(tr("Ctrl+E")));

	Task_mn = this->menuBar()->addMenu("Task");
	TaskConfig_ac = new QAction("Task Config");
	Add_Task_ac = new QAction("Add Task");
	Delete_Task_ac = new QAction("Delete Task");
	Task_mn->addAction(TaskConfig_ac);
	Task_mn->addAction(Add_Task_ac);
	Task_mn->addAction(Delete_Task_ac);
	TaskConfig_ac->setIcon(QIcon(":/CV_TEAM/icons/cameraSetting.png"));
	Add_Task_ac->setIcon(QIcon(":/CV_TEAM/icons/addTask.png"));
	Delete_Task_ac->setIcon(QIcon(":/CV_TEAM/icons/deleteTask.png"));

	Camera_mn = this->menuBar()->addMenu("Camera");
	OpenCam_ac = new QAction("Open Camera");
	Config_ac = new QAction("Config Camera");
	CloseCam_ac = new QAction("Close Camera");
	Camera_mn->addAction(OpenCam_ac);
	Camera_mn->addAction(Config_ac);
	Camera_mn->addAction(CloseCam_ac);
	OpenCam_ac->setIcon(QIcon(":/CV_TEAM/icons/cameraset.png"));
	Config_ac->setIcon(QIcon(":/CV_TEAM/icons/cameraSetting.png"));
	CloseCam_ac->setIcon(QIcon(":/CV_TEAM/icons/closeCamera.png"));

	Callibration_mn = this->menuBar()->addMenu("Callibration");
	StartCal_ac = new QAction("Open Camera");
	StopCal_ac = new QAction("Config Camera");
	StartCal_ac->setIcon(QIcon(":/CV_TEAM/icons/Openfile.png"));
	StopCal_ac->setIcon(QIcon(":/CV_TEAM/icons/Openfile.png"));

	Callibration_mn->addAction(StartCal_ac);
	Callibration_mn->addAction(StopCal_ac);


	View_mn = this->menuBar()->addMenu("View");

	System_mn = this->menuBar()->addMenu("System");
	SysConfig_ac = new QAction("Sys Config");
	SettingLanguage_ac = new QAction("Setting Language");
	SysConfig_ac->setIcon(QIcon(":/CV_TEAM/icons/cameraSetting.png"));
	SettingLanguage_ac->setIcon(QIcon(":/CV_TEAM/icons/english.png"));

	System_mn->addAction(SysConfig_ac);
	System_mn->addAction(SettingLanguage_ac);

	Setting_mn = this->menuBar()->addMenu("Setting");

	About_mn = this->menuBar()->addMenu("About");
	Liscence_ac = new QAction("Liscence");
	AboutCVTeam_ac = new QAction("About CV Team");
	Liscence_ac->setIcon(QIcon(":/CV_TEAM/icons/liscence.png"));
	AboutCVTeam_ac->setIcon(QIcon(":/CV_TEAM/icons/about.png"));

	Get_ac = new QAction("Get");
	About_mn->addAction(Liscence_ac);
	About_mn->addAction(AboutCVTeam_ac);
	About_mn->addAction(Get_ac);

	toolBar_tb = this->addToolBar("MyToolBar");

	toolBar_tb->addAction(Exit_ac);

	connect(Exit_ac, SIGNAL(triggered()), this, SLOT(on_action_Exit_triggered()));
	connect(OpenCam_ac, SIGNAL(triggered()), this, SLOT(on_action_CameraSetting_triggered()));
}

void CV_TEAM::makeCenturalWidget()
{
	for (size_t i = 0;i < 4;i++)
	{
		fr_views[i] = new FrameView();
		fr_views[i]->setParent(this);
	}
	QSplitter *splitter1 = new QSplitter();
	splitter1->setHandleWidth(0);
	splitter1->addWidget(fr_views[0]);
	splitter1->addWidget(fr_views[1]);

	QSplitter *splitter2 = new QSplitter();
	splitter2->setHandleWidth(0);
	splitter2->addWidget(fr_views[2]);
	splitter2->addWidget(fr_views[3]);

	QSplitter *splitter3 = new QSplitter(Qt::Vertical);
	splitter3->setHandleWidth(0);
	splitter3->addWidget(splitter1);
	splitter3->addWidget(splitter2);
	this->setCentralWidget(splitter3);
}

void CV_TEAM::makeDockWidget()
{
	m_TaskDockwidget = new TaskDockWidget();
	this->addDockWidget(Qt::LeftDockWidgetArea, m_TaskDockwidget);
	resizeDocks({ m_TaskDockwidget }, { 200 }, Qt::Horizontal);

	m_RTStatusWidget = new RTStatusWidget();
	this->addDockWidget(Qt::RightDockWidgetArea, m_RTStatusWidget);
	resizeDocks({ m_RTStatusWidget }, { 200 }, Qt::Horizontal);
}

void CV_TEAM::makeLogWidget()
{
}

void CV_TEAM::on_action_CameraSetting_triggered()
{
	CameraWidget *w = new CameraWidget();
	w->show();

	CommunicationWidget*ww = new CommunicationWidget();
	ww->show();
}

void CV_TEAM::on_action_Exit_triggered()
{
	qApp->quit();
}
