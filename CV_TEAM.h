#pragma once

#include <QtWidgets/QMainWindow>
#include <QSplitter>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QDesktopServices>

#include "Task\TaskDockWidget.h"
#include "Task\RTStatusWidget.h"
#include "DataModel\FrameView.h"
#include "AuxiliaryFile\Log.h"
#include "Camera\CameraWidget.h"
#include "Communication\CommunicationWidget.h"
//#define EIGEN_DONT_ALIGN_STATICALLY 
class CV_TEAM : public QMainWindow
{
	Q_OBJECT

public:
	CV_TEAM(QWidget *parent = Q_NULLPTR);

private:

	void makeMenu();
	void makeCenturalWidget();
	void makeDockWidget();
	void makeLogWidget();

	QMenu *MainAPP_mn;
	QAction *Start_ac;
	QAction *Stop_ac;
	QAction *Exit_ac;

	QMenu *Task_mn;
	QAction *TaskConfig_ac;
	QAction *Add_Task_ac;
	QAction *Delete_Task_ac;

	QMenu *Camera_mn;
	QAction *OpenCam_ac;
	QAction *Config_ac;
	QAction *CloseCam_ac;

	QMenu *Callibration_mn;
	QAction *StartCal_ac;
	QAction *StopCal_ac;

	QMenu *View_mn;
	QAction *Views_ac[4];

	QMenu *System_mn;
	QAction *SysConfig_ac;
	QAction *SettingLanguage_ac;

	QMenu *Setting_mn;

	QMenu *About_mn;
	QAction *Help_ac;
	QAction *Liscence_ac;
	QAction *AboutCVTeam_ac;
	QAction *Get_ac;

	TaskDockWidget *m_TaskDockwidget;
	RTStatusWidget *m_RTStatusWidget;
	FrameView*fr_views[4];

	QToolBar* toolBar_tb;


private slots:
	void on_action_Exit_triggered();
	void on_action_CameraSetting_triggered();
	void on_action_Help_triggerd();
	void on_action_AboutCVTeam_triggerd();
	void on_action_Get_triggerd();
};
