#include "Modbus.h"
Modbus::Modbus()
{
}
Modbus::~Modbus()
{
}
int Modbus::writeData(int address, int *data, int datacount)
{
	memset(DataSend, 0, 1024);
	int i = 0;
	for (int k = 0; k < datacount; k++)
	{
		unsigned int temp = data[k];
		unsigned short low = temp;
		unsigned short high = temp >> 16;
		DataSend[i] = low;
		DataSend[i + 1] = high;
		i = i + 2;
	}
	int error = modbus_write_registers(mb, address, datacount * 2, DataSend);
	return error;
}
int Modbus::readData(int address, int *data, int datacount)
{
	memset(DataReceive, 0, 1024);
	int error = modbus_read_registers(mb, address, datacount * 2, DataReceive);
	for (int i = 0; i < datacount; i++)
	{
		unsigned int low = DataReceive[2 * i];
		unsigned int high = DataReceive[2 * i + 1];
		unsigned int highTemp = high << 16;
		(data)[i] = (highTemp + low);
	}
	return error;
}
bool Modbus::connect(string ip, unsigned short port, int id)
{
	const char* cip;
	cip = ip.c_str(); //string to const char*
	mb = modbus_new_tcp(cip, port);
	modbus_set_slave(mb, id);
	int ret = modbus_connect(mb);
	if (ret >= 0)
	{
		struct timeval t;
		t.tv_sec = 0;
		t.tv_usec = 1000000; //????modbus???????1000????
		modbus_set_response_timeout(mb, (int)&t.tv_sec, (int)&t.tv_usec);
		return true;
	}
	else
		return false;
}
void Modbus::close()
{
	modbus_close(mb);
}