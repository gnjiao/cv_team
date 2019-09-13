#include "SerialPortIODevice.h"
SerialPortIODevice::SerialPortIODevice(QString deviceName, QObject *parent) :
	SPIODevice(deviceName, parent)
{
	//
	moveToThread(this);
}
SerialPortIODevice::~SerialPortIODevice()
{
}
void SerialPortIODevice::close()
{
	mpSerialPort->close();
	emit deviceCloseSuccessfully();
}
void SerialPortIODevice::setBaudRate(QSerialPort::BaudRate baudrate)
{
	this->mpSerialPort->setBaudRate(baudrate);
}
void SerialPortIODevice::open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity)
{
	if (mpSerialPort->isOpen())
	{
		mpSerialPort->close();
	}
	//
	mpSerialPort->setPortName(portName);
	//
	bool isOK = false;
	mpSerialPort->setBaudRate(qint32(baudRate.toInt(&isOK, 10)));
	if (!isOK) {
		emit Need2OutputInfo("Invalid baud rate!", "red");
		return;
	}
	//
	if (dataBits.compare("8") == 0) {
		mpSerialPort->setDataBits(QSerialPort::Data8);
	}
	else if (dataBits.compare("7") == 0) {
		mpSerialPort->setDataBits(QSerialPort::Data7);
	}
	else if (dataBits.compare("6") == 0) {
		mpSerialPort->setDataBits(QSerialPort::Data6);
	}
	else if (dataBits.compare("5") == 0) {
		mpSerialPort->setDataBits(QSerialPort::Data5);
	}
	else {
		emit Need2OutputInfo("Invalid baud rate!", "red");
		return;
	}
	//
	if (stopBits.compare("1") == 0) {
		mpSerialPort->setStopBits(QSerialPort::OneStop);
	}
	else if (stopBits.compare("1.5") == 0) {
		mpSerialPort->setStopBits(QSerialPort::OneAndHalfStop);
	}
	else if (stopBits.compare("2") == 0) {
		mpSerialPort->setStopBits(QSerialPort::TwoStop);
	}
	else {
		emit Need2OutputInfo("Invalid stop bits!", "red");
		return;
	}
	//
	if (parity.compare("NoParity") == 0) {
		mpSerialPort->setParity(QSerialPort::NoParity);
	}
	else if (parity.compare("EvenParity") == 0) {
		mpSerialPort->setParity(QSerialPort::EvenParity);
	}
	else if (parity.compare("OddParity") == 0) {
		mpSerialPort->setParity(QSerialPort::OddParity);
	}
	else if (parity.compare("SpaceParity") == 0) {
		mpSerialPort->setParity(QSerialPort::SpaceParity);
	}
	else if (parity.compare("MarkParity") == 0) {
		mpSerialPort->setParity(QSerialPort::MarkParity);
	}
	else {
		emit Need2OutputInfo("Invalid parity!", "red");
		return;
	}
	//
	if (mpSerialPort->open(QSerialPort::ReadWrite)) {
		emit deviceOpenSuccessfully();
		//qInfo() << "Serialport opened successfully!";
		return;
	}
	else {
		//QApplication::beep();
		emit errorStr(mpSerialPort->errorString());
		return;
	}
}
void SerialPortIODevice::writeBytes(QByteArray data)
{
	while (mpSerialPort->waitForBytesWritten(writeDelayTime()));
	qint64 ret = mpSerialPort->write(data);
	if (ret == -1) {
		//qWarning() << "Write data failed:" << mpSerialPort->errorString();
	}
	else {
		emit bytesWritten(data);
	}
}
void SerialPortIODevice::ReadAll()
{
	while (mpSerialPort->waitForReadyRead(readDelayTime()));
	QByteArray data = mpSerialPort->readAll();
	if (data.isEmpty()) {
		return;
	}
	emit bytesRead(data);
}
void SerialPortIODevice::run()
{
	mpSerialPort = new QSerialPort;
	//
	connect(mpSerialPort, SIGNAL(readyRead()), this, SLOT(ReadAll()));
	this->exec();
}