#pragma once

#include "Communication\libmodbus\modbus.h"

#include<string>


using namespace std;

class Modbus

{

public:

	Modbus();

	~Modbus();

	int writeData(int address, int *data, int datacount);

	int readData(int address, int *data, int datacount);

	bool connect(string ip, unsigned short port, int id);

	void close();

private:

	modbus_t* mb;

	unsigned short DataSend[1024];

	unsigned short DataReceive[1024];



};