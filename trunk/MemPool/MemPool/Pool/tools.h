
#pragma  once

#define DEBUGFMT  "<%s> %s(%d) : %s"
#define DEBUGARGS __FUNCTION__, __LINE__

enum LOG_TYPE
{
	LT_DEFAULT		= 0,
	LT_WARNING		= 1,
	LT_ERROR		= 2,
	LT_FATAL		= 4
};

#define PutoutLog(FILE_NAME, TYPE, FORMAT, ...) \
{ \
	char szLogType0x[32]; \
	LOG_TYPE type0x = TYPE; \
	switch( type0x ) \
{ \
	case LT_WARNING:	strcpy_s(szLogType0x, 32, "WARNING"); break; \
	case LT_ERROR:		strcpy_s(szLogType0x, 32, "ERROR"); break; \
	case LT_FATAL:		strcpy_s(szLogType0x, 32, "FATAL"); break; \
	default:			strcpy_s(szLogType0x, 32, "INFO"); break; \
} \
	char szLog0xFmt[512]; \
	sprintf_s(szLog0xFmt, 512, FORMAT,##__VA_ARGS__); \
	char szLog0x[2048]; \
	sprintf_s(szLog0x, 512, DEBUGFMT, \
	szLogType0x, DEBUGARGS, szLog0xFmt); \
	PutStringToFile(FILE_NAME, szLog0x); \
}


inline static void PutStringToFile(const char* filename, const char *msg)
{
	if(!g_bOutPut)
		return;
	char strFile[MAX_PATH];
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(strFile, "log/%s_%04d-%02d-%02d.log", filename, stTime.wYear, stTime.wMonth, stTime.wDay);

	FILE *fp;
	if( (fp=fopen(strFile,"a+")) == NULL )
	{
		return;
	}

	static int num=0;
	num++;
	char t[2048];
	sprintf(t, "\n%d(%02d-%02d %02d:%02d:%02d.%03d)",
		num, stTime.wMonth, stTime.wDay, stTime.wHour,
		stTime.wMinute, stTime.wSecond, stTime.wMilliseconds);
	fseek(fp, 0, SEEK_END);
	fwrite(t, lstrlen(t), 1, fp);
	fwrite(msg, lstrlen(msg), 1, fp);
	fclose(fp);
}