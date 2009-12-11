#include "stdafx.h"
#include "LogManager.h"

CLogManager::CLogManager()
{
	hConsole = NULL;
	bOpenFile = bDebugOutput = FALSE;
	_mkdir("Log");
}

CLogManager::~CLogManager()
{
	LogFile.closefile();
}

void CLogManager::LogInit(char* filename,int FileMode,int ConsoleMode,int OutputMode)
{
	if(FileMode==LOGFILE)		
	{
		strcpy_s(LogFileName,filename);
		OpenFile();
	}	
	if(ConsoleMode==CONSOLE)	OpenConsole();
}

void CLogManager::OpenFile()
{
	time(&time_);
	tm_ = localtime(&time_);
	
	char File[128];
	sprintf_s(File,"Log\\%04d-%02d-%02d-%s",tm_->tm_year + 1900,tm_->tm_mon + 1 ,tm_->tm_mday,LogFileName);

	LogWriteDay = tm_->tm_mday;
	bOpenFile	= LogFile.openfile(File);
}

bool CLogManager::OpenConsole()
{
	hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);

	if(hConsole == INVALID_HANDLE_VALUE)
	{
		if(::AllocConsole() == FALSE)
		{
			::FreeConsole(); return FALSE;
		}

		hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);

		if(hConsole == INVALID_HANDLE_VALUE) return FALSE;
	}
	return TRUE;
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
	
	if(hConsole) 
	{
		WriteFile(hConsole,Msg,strlen(Msg),&dwWrite,NULL);
	}	

	if(bOpenFile) LogFile.write(Msg);	
}

