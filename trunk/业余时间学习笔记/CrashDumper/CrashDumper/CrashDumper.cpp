// CrashDumper.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DumperLog.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>

int F1()
{
	int * p = NULL;
	*p = 5;
	return *p;
}

int  F2()
{
	return F1();
}
int  F3()
{
	return F2();
}

unsigned  _stdcall  ThreadFun( void * arg)
{
	//while ( 1 )
	{
#ifndef _DEBUG
		__try
		{
#endif
			F3();
#ifndef _DEBUG
		}
		__except(  Dump::DumperLog::CrashDumper( GetExceptionInformation() , GetExceptionCode() ) )
		{

		}
#endif 
	}
	_endthreadex( 0 );
	return 0;
}

#include "StackWalker.h"

class MyStackWalker : public StackWalker
{
public:
	MyStackWalker() : StackWalker() {}
	MyStackWalker(DWORD dwProcessId, HANDLE hProcess) : StackWalker(dwProcessId, hProcess) {}
	virtual void OnOutput(LPCSTR szText) { printf(szText); StackWalker::OnOutput(szText); }
};

// Exception handling and stack-walking example:
LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode)
{
	MyStackWalker sw;
	sw.ShowCallstack(GetCurrentThread(), pExp->ContextRecord);
	return EXCEPTION_EXECUTE_HANDLER;
}

void ExpTest5() { char *p = NULL; p[0] = 0; }
void ExpTest4() { ExpTest5(); }
void ExpTest3() { ExpTest4(); }
void ExpTest2() { ExpTest3(); }
void ExpTest1() { ExpTest2(); }

unsigned  _stdcall  ThreadFun1( void * arg)
{
	//while ( 1 )
	{
#ifndef _DEBUG
		__try
		{
#endif
			F3();
#ifndef _DEBUG
		}
		__except( ExpFilter(GetExceptionInformation(), GetExceptionCode()) )//Dump::DumperLog::CrashDumper( GetExceptionInformation(), GetExceptionCode()) )
		{
			;
		}
#endif 
	}
	_endthreadex( 0 );
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize( NULL );
	unsigned id;
    HANDLE Fhand = (HANDLE)_beginthreadex( NULL , 0 , ThreadFun , NULL , 0 , &id);

	WaitForSingleObject( Fhand , INFINITE );
	/*ThreadFun1(NULL);*/
	return 0;
}
