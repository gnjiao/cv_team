#pragma once
#include <QDialog>
#include <QTabWidget>
#include <QRect>
#include "LTwindow\LTwindow.h"
#include "CustomTabWidget.h"
#include "SerialPort\SerialPortWidget.h"
#include "TcpServer\TcpServerWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
class CommunicationWidget : public LTwindow
{
	Q_OBJECT

public:
	CommunicationWidget(QWidget *parent = Q_NULLPTR);
	~CommunicationWidget();

private:
	void makeTabWidget();
	void makeMainLayout();
	QTabWidget *m_tabWidget;
	TcpServerWidget *m_TcpServerWidget;
	QWidget *UdpWidget;
	SerialPortWidget *m_SerialPortWidget;
	QWidget *ModbusWidget;
	QRect location;
	bool max;

	CustomTabWidget *m_customTab;
};
