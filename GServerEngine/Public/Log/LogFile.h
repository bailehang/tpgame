/*
* Brief :  一个日记打开文件类
* Author:  Expter  
* Date  :  [2009-10-21]                          
**/
#ifndef _LOGFILE_H__
#define _LOGFILE_H__

class CLogFile
{
private:

	HANDLE hFile;
	DWORD Writes;

public:

	CLogFile() { hFile = NULL; }

	void __stdcall closefile()
	{
		if(hFile) CloseHandle(hFile);	
		
		hFile = NULL;	
	} 
	
	bool __stdcall openfile(char* filename)
	{
		closefile();

		hFile = CreateFileA(filename,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				
		SetFilePointer(hFile,0,NULL,FILE_END);

		return TRUE;
	};
	
	void __stdcall write(char* msg)
	{
		WriteFile(hFile,msg,(DWORD)strlen(msg),&Writes,NULL);
	}
};

#endif