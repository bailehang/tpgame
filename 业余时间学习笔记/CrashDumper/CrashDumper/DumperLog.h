#pragma once

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "dbghelp.lib")
namespace Dump
{

	class DumperLog
	{
	public:
		DumperLog(void);

		virtual ~DumperLog(void);

	public:
		static int CrashDumper(LPEXCEPTION_POINTERS lpEp, DWORD dwExceptionCode , char * dumpfilename = 0);
		static void DumpCallStack(PCONTEXT pcontext , BOOL bWriteVarables , HANDLE hProcess);
	};
}