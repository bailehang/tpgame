#include "stdafx.h"
#include "LogManager.h"

/// 调用方式 先LogInit,在write信息
/// GetInstObj(Obj)
CLogManager::CLogManager()
{
	_mkdir("Log");
}

CLogManager::~CLogManager()
{
	LogFile.closefile();
}

void CLogManager::LogInit(char* filename,int FileMode)
{
	if(FileMode==LOGFILE)		
	{
		strcpy_s(LogFileName,filename);
		OpenFile();
	}
}

void CLogManager::OpenFile()
{
	time(&time_);
	tm_ = localtime(&time_);
	
	char File[128];
	sprintf_s(File,"Log\\%04d-%02d-%02d-%s",tm_->tm_year + 1900,tm_->tm_mon + 1 ,tm_->tm_mday,LogFileName);

	bOpenFile	= LogFile.openfile(File);
}

char* CLogManager::GetTime()
{
	time(&time_);
	tm_ = localtime(&time_);
	
	sprintf_s(Time,"<%d-%d-%d %d:%d:%d> ",tm_->tm_year + 1900,tm_->tm_mon + 1 ,tm_->tm_mday,tm_->tm_hour,tm_->tm_min,tm_->tm_sec);
	
	return Time;
}

void CLogManager::Write(char* fmt ,...)
{
	va_start(args,fmt);
	vsprintf_s(LogMsg,fmt,args);
	va_end(args);
	
	sprintf_s(Msg,"%s%s\r\n",GetTime(),LogMsg);
	
	if(bOpenFile) LogFile.write(Msg);	
}

