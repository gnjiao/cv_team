#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "AuxiliaryFile/include/spdlog/spdlog.h"
#include "ThreadSafeQueue.h"

#define Loger Log::Instance()
bool CreatFile(string path = "DailyLog");
enum LogType
{
	NORMAL_MESSAGE,
	ERROR_MESSAGE,
	WARN_MESSAGE,
	CRITICAL_MESSAGE
};
struct LogStruct
{
	string m_str;
	LogType m_type;
};
class Log
{
private:
	Log();
public:
	~Log();
	ThreadSafeQueue<LogStruct> m_QueueMessage;
	static shared_ptr<Log> Instance();
	void WriteLog(string logstring, LogType type);
	static void InitialLog();
	static void CloseLog();
};