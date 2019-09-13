#include "TcpIODevice.h"
TcpIODevice::TcpIODevice(QString deviceName, QObject * parent)
{
	this->DeviceName = deviceName;
}
TcpIODevice::~TcpIODevice()
{
}
void TcpIODevice::open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort)
{
}
void TcpIODevice::open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity)
{
}