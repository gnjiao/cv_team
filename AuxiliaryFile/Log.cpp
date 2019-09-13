#include <boost/filesystem.hpp>
#include <boost/system/system_error.hpp>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "Log.h"

#define BOOST_FILESYSTEM_NO_DEPRECATED
bool thread_log = 0;
std::thread* t_log;
mutex m_mutex_log;
bool CreatFile(string file)
{
	boost::filesystem::path p(file);
	if (is_directory(p))
	{
		return true;
	}
	else
	{
		boost::system::error_code ec;
		return create_directory(p, ec);
	}
}
void AutoWriteThread()
{
	try
	{
		size_t q_size = 4096;
		spdlog::set_async_mode(q_size);
		auto daily_logger = spdlog::daily_logger_mt("CV_Team", "DailyLog/log", 2, 0);
		while (thread_log || Loger->m_QueueMessage.size())
		{
			LogStruct *plog = Loger->m_QueueMessage.Dequeue();
			while (nullptr != plog)
			{
				switch (plog->m_type)
				{
				case LogType::NORMAL_MESSAGE:
					daily_logger->info(plog->m_str);
					break;
				case LogType::ERROR_MESSAGE:
					daily_logger->error(plog->m_str);
					break;
				case LogType::WARN_MESSAGE:
					daily_logger->warn(plog->m_str);
					break;
				case LogType::CRITICAL_MESSAGE:
					daily_logger->critical(plog->m_str);
					break;
				default:
					break;
				}
				delete plog;
				plog = Loger->m_QueueMessage.Dequeue();
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info("End of log."); });
		spdlog::drop_all();
	}
	catch (const spdlog::spdlog_ex &ex)
	{
		string s = ex.what();
	}
	catch (const std::exception&ex)
	{
		string s = ex.what();
	}
	return;
}
Log::Log()
{
}
Log::~Log()
{
}
shared_ptr<Log> Log::Instance()
{
	lock_guard<mutex> lk(m_mutex_log);
	static shared_ptr<Log> instance(new Log());
	return instance;
}
void Log::WriteLog(string logstring, LogType type)
{
	LogStruct *mes = new LogStruct();
	mes->m_str = logstring;
	mes->m_type = type;
	m_QueueMessage.Enqueue(mes);
}
void Log::InitialLog()
{
	if (thread_log)
		return;
	if (!CreatFile())
	{
		throw runtime_error("Failed in creating logs file");
	}
	Instance();
	thread_log = true;
	t_log = new std::thread(AutoWriteThread);
}
void Log::CloseLog()
{
	if (thread_log)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		thread_log = false;
		t_log->join();
	}
}