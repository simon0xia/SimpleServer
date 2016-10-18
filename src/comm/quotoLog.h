#ifndef __MIRRORLOG_H__
#define __MIRRORLOG_H__

#include <iostream>
#include <mutex>
#include <fstream>
#include <sstream>
#include <ctime>
#include <excpt.h>

//日志级别，共4种，分别为fatal, error, warning, info
enum LogVerbossLevel
{
	lvl_info,
	lvl_waring,
	lvl_error,
	lvl_fatal
};


//获取时间变量，到天，为GET_TIME_OF_DAT,到秒为TIME获取昨天为GET_TIME_OF_YESTERDAY
#define GET_TIME_OF_YESTERDAY	2
#define GET_TIME_OF_DAY			1
#define TIME					0

class QuotoLog
{
public:
	static class QuotoLog& get_logClass()
	{
		static QuotoLog mirLog;   //局部静态变量  
		return mirLog;
	}

	bool init(int _logLevel, std::string _logFileName = "");
	void append(int _logLevel, std::string FunctionName, int errorcode);
	void append(int _logLevel, std::string FunctionName, int line, int errorcode);
	void flashLog(int _logLevel, std::string errMsg);

private:
	QuotoLog() { ; }	//构造函数私有
	QuotoLog(const QuotoLog &);
	QuotoLog & operator = (const QuotoLog &);

	void delete_oldLog();
	std::string get_time(int flag = TIME);

	std::string m_logfilename;
	int m_loglevel;
	std::mutex lock;
};

#define LogIns QuotoLog::get_logClass()

#endif //__MIRRORLOG_H__
