/*
* Brief :  一个日记写文件类
* Author:  Expter  
* Date  :  [2009-10-21]                          
**/
#pragma once

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <direct.h>

#include "LogFile.h"

enum LOGMODE{ LOGFILE=1,CONSOLE,OUTPUT };

class CLogManager
{
public:
	CLogManager();
	~CLogManager();

	void LogInit(char* filename="log.txt",int FileMode=LOGFILE);		
	
	void Write(char* fmt ,...);
	
private:
	void	OpenFile();	
	char*	GetTime();


private:

	struct tm	*tm_;
	time_t		time_;
	bool		bOpenFile;
	char		LogFileName[MAX_PATH];
	char		Time[128];
	char		LogMsg[1024];
	char		Msg[1152];
	va_list		args;
	DWORD		dwWrite;

	CLogFile	LogFile;
	
};
