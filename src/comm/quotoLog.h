#ifndef __MIRRORLOG_H__
#define __MIRRORLOG_H__

#include <iostream>
#include <mutex>
#include <fstream>
#include <sstream>
#include <ctime>
#include <excpt.h>

//��־���𣬹�4�֣��ֱ�Ϊfatal, error, warning, info
enum LogVerbossLevel
{
	lvl_info,
	lvl_waring,
	lvl_error,
	lvl_fatal
};


//��ȡʱ����������죬ΪGET_TIME_OF_DAT,����ΪTIME��ȡ����ΪGET_TIME_OF_YESTERDAY
#define GET_TIME_OF_YESTERDAY	2
#define GET_TIME_OF_DAY			1
#define TIME					0

class QuotoLog
{
public:
	static class QuotoLog& get_logClass()
	{
		static QuotoLog mirLog;   //�ֲ���̬����  
		return mirLog;
	}

	bool init(int _logLevel, std::string _logFileName = "");
	void append(int _logLevel, std::string FunctionName, int errorcode);
	void append(int _logLevel, std::string FunctionName, int line, int errorcode);
	void flashLog(int _logLevel, std::string errMsg);

private:
	QuotoLog() { ; }	//���캯��˽��
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
