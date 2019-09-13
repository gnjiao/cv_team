#pragma once
#include <QDockWidget>
#include <QVector>
#include <QTabWidget>
#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QList>
#include <QIcon>
#include <QTreeWidget>
#include "Inspection\Inspection.h"
#include "Inspection\InspectionDialog.h"
class TaskDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	TaskDockWidget(QWidget *parent = Q_NULLPTR);
	~TaskDockWidget();
	private slots:
	void on_TabWidget_Changed(int);
	void on_Add_btn_Clicked(bool);
	void on_Delete_btn_Clicked(bool);
	void on_itemDoubleClicked(QTreeWidgetItem*);
private:
	void makeTaskWidget();
	void makeTreeWidget();
	void makeLayout();
	void initWidget();
	void LoadInspection();
	QPushButton *Add_btn;
	QPushButton *Delete_btn;
	int m_InspectionKey = 0;
	QTabWidget *TaskWidget;
	QVector<QTreeWidget*>TreeWidgetVector;
	QVector<QVector<QTreeWidgetItem*>>InspectionItem;
	QList<InspectionDialog*> m_InsDialogs;
};