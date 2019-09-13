#include "Inspection.h"
#include <iostream>
#include <thread>
#include <direct.h>

void Inspection::MakeDir()
{
	std::string dir = config_dir;
	_mkdir(dir.c_str());
	dir = config_dir + "\\" + m_name;
	_mkdir(dir.c_str());
}
NodeState Inspection::execute()
{
	m_toolBlock->Reset();
	auto result = m_toolBlock->execute();
	SetReuslt(m_toolBlock->getResult());
	return result;
}