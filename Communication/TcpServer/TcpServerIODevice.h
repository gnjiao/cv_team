#pragma once
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>
#include <QDebug>
#include <QHostAddress>
#include <QTimer>
#include "TcpIODevice.h"
class TcpServerIODevice : public TcpIODevice
{
	Q_OBJECT
public:
	TcpServerIODevice(QString deviceName = QString("TcpServer"), QObject *parent = Q_NULLPTR);
	~TcpServerIODevice();
	virtual bool isOpen() { return mpTcpServer->isListening(); }
	virtual QString errorString() { return mpTcpServer->errorString(); }
	QTcpSocket *getCurrentTcpClient();
	QList <QTcpSocket *>getClients();
	void setCurrentClient(QTcpSocket*);
	void changedCurrentScoket(QString address, QString port);
	public slots:
	virtual void writeBytes(QByteArray data);
	virtual void open(QString serverAddress, QString serverPort);
	virtual void close();
protected:
	void run();
private:
	QTimer *stateTimer = nullptr;
	QTcpSocket *currentTcpClient = nullptr;
	QTcpServer *mpTcpServer = nullptr;
	QList <QTcpSocket *> clients;
	private slots:
	void readBytes();
	void newConnecting();
	void checkState();
signals:
	void clientsChanged(QList <QTcpSocket *> clients);
};
#define UI_STR_DEVICE_IS_NULL tr("?υτ??????!")
Q_DECLARE_METATYPE(QList <QTcpSocket *>)