#pragma once
#include <QDockWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class StatusWidget : public QWidget
{
	Q_OBJECT
public:
	StatusWidget(int key,QWidget *parent = Q_NULLPTR);
	~StatusWidget();
	void setOKorNG(bool);
	void setExecCount(int);
	void setOKCount(int);
	void setNGCount(int);
	void setNGInspection(int);
private:
	int TaskKey;
	QLabel *OKorNG_lb;
	QLabel *ExecCount_lb;
	QLabel *OKCount_lb;
	QLabel *OKRatio_lb;
	QLabel *NGCount_lb;
	QLabel *NGRatio_lb;
	QLabel *NGInspection_lb;
	
};

class RTStatusWidget : public QDockWidget
{
	Q_OBJECT

public:
	RTStatusWidget(QWidget *parent = Q_NULLPTR);
	~RTStatusWidget();

private:
	
	QVector<StatusWidget*>m_statusWidgets;


};
