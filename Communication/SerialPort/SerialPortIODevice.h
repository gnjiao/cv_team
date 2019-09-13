#pragma once
#include "SPIODevice.h"
#include <QSerialPort>
class SerialPortIODevice : public SPIODevice
{
	Q_OBJECT
public:
	SerialPortIODevice(QString deviceName = QString("SerialPort"), QObject *parent = Q_NULLPTR);
	~SerialPortIODevice() override;
	bool isOpen() override { return mpSerialPort->isOpen(); }
	QString errorString() override { return mpSerialPort->errorString(); }
	void setBaudRate(QSerialPort::BaudRate baudrate);
	public slots:
	void close() override;
	void open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity) override;
	void writeBytes(QByteArray data) override;
	void ReadAll();
protected:
	void run() override;
private:
	//
	QSerialPort *mpSerialPort = nullptr;
signals:
	//
	void SerialPortCloseSuccessfully();
	//
	void SerialPortOpenSuccessfully();
	//
	void Need2OutputInfo(QString info, QString color = "black", bool prefix = true);
};