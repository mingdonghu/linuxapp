#include "logmodule.h"

#include <time.h>
#include <string.h>

#ifndef LINUX
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
#else
#include <stdlib.h>
#endif

//使用vswprintf会出现奔溃的情况如果，传入数据大于 VA_PARAMETER_MAX 就会出现崩溃
#define  VA_PARAMETER_MAX  1024 * 2

LogModule* LogModule::s_plog_module_ = NULL;


LogModule* LogModule::GetInstance(__in const char* filename, __in const char* funcname, __in int lineno,LogLevel level,ILogRealization*plog) {
	if (s_plog_module_ == NULL) {
		s_plog_module_ = new LogModule();
	}
	s_plog_module_->logInfo_.str_filename = filename;
	s_plog_module_->logInfo_.str_funcname = funcname;
	s_plog_module_->logInfo_.n_linenumber = lineno;
	s_plog_module_->logInfo_.loglevel = level;
	
	if (plog != NULL) {
		s_plog_module_->p_realization_->free();
		s_plog_module_->p_realization_ = plog;
	}
	return s_plog_module_;
}

LogModule* LogModule::GetInstance(LogLevel level, ILogRealization*plog) {
	if (s_plog_module_ == NULL) {
		s_plog_module_ = new LogModule();
	}
	s_plog_module_->logInfo_.loglevel = level;
	
	if (plog != NULL) {
		s_plog_module_->p_realization_->free();
		s_plog_module_->p_realization_ = plog;
	}
	return s_plog_module_;
}

LogModule::LogModule() {
	logInfo_.n_linenumber = -1;
	logInfo_.str_filename = "";
	logInfo_.str_funcname = "";
#ifndef LINUX
	p_realization = new LogOutputString();
#else
	p_realization_ = new LogPrint();
#endif
	InitLock();
}

LogModule::~LogModule() {
	RealseLock();
}

void LogModule::LogPrintInf(const char* format,...) {
	Lock();
	if (p_realization_) {
		std::string str_temp;
		// manufacture
		str_temp.append("[LOG]");
		//LogLevel
		str_temp.append(GetLevelValue(logInfo_.loglevel));
		switch (logInfo_.loglevel) {
			case DEBUG_LEVEL: {
				//时间戳 uint is seconds
				char s_stamp[100] = {0};
				uint64_t timestamp = GetCurrentLocalTimeStamp();
#ifdef __LP64__
        snprintf(s_stamp, 100, "[%lu.%lu]", (timestamp/1000000000), (timestamp%1000000000));
#else
#ifdef _WIN64
    		snprintf(s_stamp, 100, "[%lu.%lu]", (timestamp/1000000000), (timestamp%1000000000));
#else
    		snprintf(s_stamp, 100, "[%llu.%llu]", (timestamp/1000000000), (timestamp%1000000000));
#endif
#endif

				str_temp.append(s_stamp);
			}
				break;
			default: {
				//时间   [week month day hours:minutes:seconds year]
				str_temp.append(GetFormatValue(GetCurrentTime()));
			}
				break;
		}
	
		//文件名称
		str_temp.append(GetFormatValue(logInfo_.str_filename));
		str_temp.append(GetFormatValue(logInfo_.str_funcname));
		//行号
		str_temp.append(GetFormatValue(logInfo_.n_linenumber));

		va_list ptr;
		va_start(ptr, format);
		char c_value[VA_PARAMETER_MAX] = {0};
		vsnprintf(c_value,sizeof(c_value),format,ptr);
		va_end(ptr);

		str_temp.append(GetFormatValue(c_value));

		p_realization_->LogPrintInf(str_temp.c_str());
	}
	UnLock();
}

void LogModule::LogPrintNoLocationInf(const char* format,...) {
	Lock();
	if (p_realization_) {
		std::string str_temp;
		// manufacture
		str_temp.append("[LOG]");
		//LogLevel
		str_temp.append(GetLevelValue(logInfo_.loglevel));
		
		//时间戳 uint is seconds
		char s_stamp[100] = {0};
		uint64_t timestamp = GetCurrentLocalTimeStamp();
#ifdef __LP64__
		snprintf(s_stamp, 100, "[%lu.%lu]", (timestamp/1000000000), (timestamp%1000000000));
#else
#ifdef _WIN64
    snprintf(s_stamp, 100, "[%lu.%lu]", (timestamp/1000000000), (timestamp%1000000000));
#else
    snprintf(s_stamp, 100, "[%llu.%llu]", (timestamp/1000000000), (timestamp%1000000000));
#endif
#endif

		str_temp.append(s_stamp);
		
		va_list ptr;
		va_start(ptr, format);
		char c_value[VA_PARAMETER_MAX] = {0};
		vsnprintf(c_value,sizeof(c_value),format,ptr);
		va_end(ptr);

		str_temp.append(GetFormatValue(c_value));

		p_realization_->LogPrintInf(str_temp.c_str());
	}
	UnLock();
}

void LogModule::InitLock() {
#ifndef LINUX
	InitializeCriticalSection(&mutex_lock_);
#else
  pthread_mutex_init(&mutex_lock_,NULL);
#endif
}

void LogModule::RealseLock() {
#ifndef LINUX
	DeleteCriticalSection(&mutex_lock_);
#else
	pthread_mutex_unlock(&mutex_lock_);
#endif
}

void LogModule::Lock() {
#ifndef LINUX
	EnterCriticalSection(&mutex_lock_);
#else
	pthread_mutex_lock(&mutex_lock_);
#endif
}

void LogModule::UnLock() {
#ifndef LINUX
	LeaveCriticalSection(&mutex_lock_);
#else
	pthread_mutex_unlock(&mutex_lock_);
#endif
}

std::string LogModule::GetCurrentTime() {
	std::string curr_time;
#if 0
	//Current date/time based on current time
	time_t now = time(0);               // 英文风格时间字符格式 
	// Convert current time to string
	curr_time.assign(ctime(&now));
	// Last charactor of currentTime is "\n", so remove it
	std::string current_time = curr_time.substr(0, curr_time.size()-1);
	return current_time;
#else
  char stdtime_str[50] = {0};  // ISO 国际时间
	time_t std_time = 0;
	struct tm* local_time = NULL;
	std_time = time(NULL);
	local_time = localtime(&std_time);
	snprintf(stdtime_str, 50, "%d-%2d-%2d,%2d:%2d:%2d", 
	local_time->tm_year+1900, local_time->tm_mon+1, local_time->tm_mday,
	local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
	curr_time.assign(stdtime_str);
  return curr_time;
#endif
}

std::string LogModule::GetFormatValue(std::string str_value) {
	std::string str_temp;
	str_temp.append("[");
	str_temp.append(str_value);
	str_temp.append("]");
	return str_temp;
}

std::string LogModule::GetFormatValue(int n_value) {
	std::string str_temp;
	str_temp.append("[");
	char c_value[16];
	sprintf(c_value,"%d",n_value);
	str_temp.append(c_value);
	str_temp.append("]");
	return str_temp;
}

std::string  LogModule::GetLevelValue(int level){
	std::string tmp;
	switch (level) {
	case DEBUG_LEVEL:
		tmp = "DEBUG";
		break;
	case WARNING_LEVEL:
		tmp = "WARN";
		break;
	case ERROR_LEVEL:
		tmp = "ERROR";
		break;
	case INFO_LEVEL:
		tmp = "INFO";
		break;
	default:
		tmp = "UnKnown";
		break;
	}
	std::string str_temp;
	str_temp.append("[");
	str_temp.append(tmp);
	str_temp.append("]");
	return str_temp;
}

void LogPrint::Initializion(const char* path) {
	printf("%s", path);
	return ;
}

void LogPrint::free(ILogRealization *plogger) {
	LogPrint* pOutput = static_cast<LogPrint*>(plogger);
	if (pOutput != NULL) {
		delete pOutput;
	}
}

void LogPrint::LogPrintInf(const char* str) {
#ifdef ENABLE_CONSOLE_LOG_DISPLAY
	printf("%s\r\n", str);
#endif

#ifdef ENABLE_LOG_WRITE_TO_FILE
  std::string log_file_name = GetLogFilePathName();
	FILE *fp = fopen(log_file_name.c_str() ,"a");
	if(!fp) {
		printf("%s open filed!\n", log_file_name.c_str());
		return ;
	}
	printf_s(fp,str);
	printf_s(fp,"\r\n");
	fclose(fp);
#endif
}

/********************* (C) COPYRIGHT SHENZHEN LDROBOT CO., LTD *******END OF FILE ********/