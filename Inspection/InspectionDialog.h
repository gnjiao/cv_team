#pragma once
#include <QDialog>
#include <QToolBar>
#include <QFileDialog>
#include <QList>
#include <QGraphicsItem>
#include <QSplitter>
#include <QTextBrowser>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

#include <mutex>
#include "ToolBlockWidget.h"
#include "ToolBoxWidget.h"
#include "DataModel\FrameView.h"
#include "FileWidget.h"
#include "AuxiliaryFile\TitleBar.h"
#include "DisplayWidget.h"
class InspectionDialog : public QDialog
{
	Q_OBJECT
public:
	InspectionDialog(QWidget *parent = Q_NULLPTR);
	~InspectionDialog();
	Inspection *m_inspection;
	void setInspection(Inspection *ins);
	void updateTreeWidget();
	QList<QGraphicsItem*> getShowItem();
	FrameView *getFrameView();
	void setTitle(QString);
private slots:
	void on_action_PlayImage_triggered();
	void on_action_PlayCamera_triggered();
	void on_action_Repeat_triggered();
	void on_action_Display_ac_triggered();
	void on_action_Output_triggered();
	void on_action_Exit_triggered();
	void on_ToolBlockItemDoubleClicked(QTreeWidgetItem * item);
	void onButtonMinClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();
	void on_SelectFile_Clicked(QString);
	void on_Next_btn_Clicked(QString);
	void on_Last_btn_Clicked(QString);
	void on_Start_btn_Clicked();
	void on_Stop_btn_Clicked();

	void on_DisplaySettingOK();
	void on_excuteFinished_Triggered();
private:
	void makeToolBar();
	void makeMainLayout();
	void showAllOutputItems();
	void updateResult();

	void AutoRun();

	QToolBar *toolBar;
	QAction *PlayImage_ac;
	QAction *PlayCamera_ac;
	QAction *Repeat_ac;
	QAction *Display_ac;
	QAction *Output_ac;
	QAction *Exit_ac;
	TitleBar *m_title;
	QFileDialog *OpenFileDialog;
	ToolBoxWidget *toolBoxWidget;
	ToolBlockWidget *toolBlockWidget;
	DisplayWidget *m_displayWidget;
	FrameView *fr_view;
	QList<QGraphicsItem*> m_showItems;
	FileWidget *m_fileWidget;

	std::mutex mtx;
	QRect location;
	string m_path;
	bool max;
	bool runFlag = false;
	bool ready = false;

signals:
	void excuteFinished();

};