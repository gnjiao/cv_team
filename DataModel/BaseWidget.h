#pragma once
#include <QDialog>
#include <QWidget>
#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QSplitter>
#include <QColor>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTabWidget>
#include <QFrame>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "DataModel\FrameView.h"
class BaseWidget : public QDialog
{
	Q_OBJECT
public:
	BaseWidget(QWidget *parent = Q_NULLPTR);
	~BaseWidget();
protected:
	QAction *action_openFile;
	QAction *action_play;
	QAction *action_edit;
	QAction *action_editCancel;
	QToolBar *toolBar; //
	QTabWidget *TabWidget;
	QWidget *BaseParamWidget;

	QWidget *SetItemWidget;

	QWidget *OutputWidget;
	QLabel *botMsgBox;

	FrameView *fr_view;
	virtual void doOpenFile(cv::Mat img) {}
	virtual void doEdit(bool b) {}
	virtual void doEditSave() {}
	virtual void doEditCancel() {}
	virtual void doPlay() = 0;
	virtual void doOk() = 0;
	virtual void doCancel() = 0;
private slots:
	void on_action_openFile_triggered();
	void on_action_play_triggered();
	void on_action_edit_toggled(bool b);
	void on_action_editCancel_triggered();
	void on_OK_btn_clicked();
	void on_Cancel_btn_clicked();
private:
	void makeToolBar();
	void makeWidget();
	QPushButton *OK_btn;
	QPushButton *Cancel_btn;
	QFileDialog *openFileDlg;
};