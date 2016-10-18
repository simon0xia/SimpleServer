#include "quotoLog.h"
#include <QDateTime>

std::string error2str(int code)
{
	return "fix it later";
}

bool QuotoLog::init(int _logLevel, std::string _logFileName)
{
	QDateTime dt = QDateTime::currentDateTime();
	std::string str = "./logs/" + _logFileName + "-" + dt.toString("yyyy-MM-dd").toStdString();

	m_loglevel = _logLevel;
	m_logfilename = str;

	return true;
}

void QuotoLog::append(int _logLevel, std::string FunctionName, int errorcode)
{	
	//写日志先判断运行级别
	if (_logLevel < m_loglevel)
	{
		return;
	}

	lock.lock();
	std::fstream out(m_logfilename.c_str(), std::ios::out | std::ios::app);
	out << get_time(TIME);
	out << std::string(" ") << _logLevel << std::string(" ") << FunctionName << std::string("\t");
	out << errorcode << std::string("\t") << error2str(errorcode) << std::endl;

	out.close();
	lock.unlock();
}

void QuotoLog::append(int _logLevel, std::string FunctionName, int line, int errorcode)
{
	//写日志先判断运行级别
	if (_logLevel < m_loglevel)
	{
		return;
	}

	lock.lock();
	std::fstream out(m_logfilename.c_str(), std::ios::out | std::ios::app);
	out << get_time(TIME);
	out << std::string(" ") << _logLevel << std::string(" ") << FunctionName << std::string(":") << line;
	out << errorcode << std::string("\t") << error2str(errorcode) << std::endl;

	out.close();
	lock.unlock();
}

void QuotoLog::flashLog(int _logLevel, std::string errMsg)
{
	//写日志先判断运行级别
	if (_logLevel < m_loglevel)
	{
		return;
	}

	lock.lock();
	std::fstream out(m_logfilename.c_str(), std::ios::out | std::ios::app);
	out << get_time(TIME);
	out << std::string(" ") << _logLevel << std::string(" ") << errMsg;

	out.close();
	lock.unlock();
}

std::string QuotoLog::get_time(int flag)
{
	char tmp[64];
	tm tm;
	
	if (flag == TIME)
	{
		time_t t = time(0);
		localtime_s(&tm, &t);
		strftime(tmp, sizeof(tmp), "%m-%d %H:%M:%S", &tm);
	}
	else if (flag == GET_TIME_OF_DAY)
	{
		time_t t = time(0);
		localtime_s(&tm, &t);
		strftime(tmp, sizeof(tmp), "%m-%d", &tm);
	}
	else if (flag == GET_TIME_OF_YESTERDAY)
	{
		time_t t = time(0) - 24 * 3600;
		localtime_s(&tm, &t);
		strftime(tmp, sizeof(tmp), "%m-%d", &tm);
	}
	else
	{
		time_t t = time(0);
		localtime_s(&tm, &t);
		strftime(tmp, sizeof(tmp), "%m-%d %H:%M:%S", &tm);
	}

	return std::string(tmp);
}


