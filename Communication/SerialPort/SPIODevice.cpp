#include "SPIODevice.h"
SPIODevice::SPIODevice(QString deviceName, QObject *parent) :QThread(parent)
{
	DeviceName = deviceName;
}
SPIODevice::~SPIODevice()
{
}
void SPIODevice::open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort)
{
	Q_UNUSED(hostAddress);
	Q_UNUSED(hostPort);
	Q_UNUSED(peerAddress);
	Q_UNUSED(peerPort);
}
void SPIODevice::open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity)
{
	Q_UNUSED(portName);
	Q_UNUSED(baudRate);
	Q_UNUSED(dataBits);
	Q_UNUSED(stopBits);
	Q_UNUSED(parity);
}