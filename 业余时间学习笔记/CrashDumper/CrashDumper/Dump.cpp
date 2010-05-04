#include <iostream>
#include <process.h>
#include <C:\Program Files\Microsoft SDKs\Windows\v6.0A\include\DbgHelp.h>

#define WriteLogFile(filename,context)	\
	{ \
		ofstream out(filename,ios::app);	\
		out << context << std::endl;	\
		out.close();	\
	}

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

namespace Dump
{


	DumperLog::DumperLog(void)
	{

	}

	DumperLog::~DumperLog(void)
	{

	}


	void DumperLog::DumpCallStack(PCONTEXT pcontext , BOOL bWriteVarables , HANDLE hProcess)
	{
		WriteLogFile( "dumplog.txt" ,"\r\n Call Stack:\r\n\r\n" );

		DWORD machineType =0;

		STACKFRAME sf;
		memset( & sf, 0 , sizeof(sf) );

		sf.AddrPC.Offset = pcontext->Eip;
		sf.AddrPC.Mode   =  AddrModeFlat;
		sf.AddrStack.Offset = pcontext->Esp;
		sf.AddrStack.Mode= AddrModeFlat;
		sf.AddrFrame.Offset = pcontext->Esp;
		sf.AddrFrame.Mode = AddrModeFlat;

		machineType = IMAGE_FILE_MACHINE_I386;

		int i = 0;


		for(;;)
		{
			if( !StackWalk(machineType,hProcess,GetCurrentThread(),&sf,pcontext ,0,SymFunctionTableAccess,SymGetModuleBase,0))
			{
				break;
			}

			if ( 0 == sf.AddrFrame.Offset)
			{ 
				break;
			}
			i++;

			BYTE symbolBuf[ sizeof( SYMBOL_INFO) + 1024];
			PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbolBuf;

			pSymbol->SizeOfStruct = sizeof( symbolBuf);
			pSymbol->MaxNameLen   = 1024;


			DWORD64 symDisplacement = 0;
			/// 获取符号
			if ( SymFromAddr( hProcess,sf.AddrPC.Offset ,&symDisplacement,pSymbol) )
			{
				if ( bWriteVarables )
				{
					WriteLogFile("dumplog.txt",TEXT("%4d Function : %hs\r\n\t\t\t\t\t(+0x%I64x)##[Address : 0x%08x]##[Frame : 0x%08x]"),i,
						pSymbol->Name,symDisplacement,sf.AddrPC.Offset,sf.AddrFrame.Offset);
				}
				else
				{
					//WriteLogFile("dumplog.txt",TEXT("%4d Function :%hs"),i,pSymbol->Name);
				}
			}
			/// 没有找到
			else
			{
				TCHAR szModule[MAX_PATH] = TEXT("");
				TCHAR section[IMAGE_SIZEOF_SHORT_NAME +1 ] = TEXT("");
				DWORD offset = 0;

				MEMORY_BASIC_INFORMATION mbi;

				do 
				{
					if ( !VirtualQuery( UlongToPtr( sf.AddrPC.Offset) ,&mbi , sizeof(mbi)))
					{
						break;
					}

					HMODULE hMod = (HMODULE)mbi.AllocationBase;

					int  num = 0;
					if ( !( num=GetModuleFileName( hMod , szModule , static_cast<DWORD>( num )) ) )
					{
						cerr << " num = " << num << std::endl;
						break;
					}

					/// dos 文件头
					PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;

					/// nt 文件头
					PIMAGE_NT_HEADERS pNtHdr =(PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);

					/// section头
					PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );

					/// rva地址
					DWORD  rva = (DWORD)sf.AddrPC.Offset - PtrToUlong( hMod);

					for ( WORD i = 0 ; i < pNtHdr->FileHeader.NumberOfSections ; i++ , pSection++)
					{
						DWORD sectionStar = pSection->VirtualAddress;
						DWORD sectionEnd  = sectionStar + max( pSection->SizeOfRawData ,pSection->Misc.VirtualSize);


						// rva落在了section
						if ( (rva >= sectionStar) && (rva <= sectionEnd))
						{
							_tcsncpy( section , reinterpret_cast<LPCTSTR>(pSection->Name),IMAGE_SIZEOF_SHORT_NAME);
							section[ IMAGE_SIZEOF_SHORT_NAME ] = 0;
							offset = 0;
							break;
						}
					}

				} while (0);

				if ( bWriteVarables )
				{
					char str[1000];
					sprintf_s(str,"%4d Function : [Module  :%s ] ## [ Section: %8s offset 0: x%08x] ",i,szModule,section,offset);;//"%4d Function : %hs\r\n\t\t\t\t\t(+0x%I64x)##[Address : 0x%08x]##[Frame : 0x%08x]",i,pSymbol->Name,symDisplacement,sf.AddrPC.Offset,sf.AddrFrame.Offset);
					WriteLogFile("dumplog.txt",str);
				}
				else
				{
					//WriteLogFile("dumplog.txt",TEXT("%4d Function : [Module  :%s ] ## [ Section: %8s offset 0: x%08x] "),i,szModule,section,offset);
				}
			}

			if ( bWriteVarables )
			{
				IMAGEHLP_LINE LlineInfo = { sizeof(IMAGEHLP_LINE) };
				DWORD         dwLineDispment;
				if (SymGetLineFromAddr(hProcess,sf.AddrPC.Offset,&dwLineDispment,&LlineInfo))
				{
					WriteLogFile("dumplog.txt",TEXT("[Source File  : %s ]"),LlineInfo.FileName);
					WriteLogFile("dumplog.txt",TEXT("[Source Line  : %d ]"),LlineInfo.LineNumber);
				}
			}

		}


	}


	int DumperLog::CrashDumper(LPEXCEPTION_POINTERS lpEp, DWORD dwExceptionCode, char *dumpfilename)
	{

		HANDLE			  hProcess  = GetCurrentProcess();
		PEXCEPTION_RECORD lpx       = lpEp->ExceptionRecord;
		PCONTEXT          lpContext = lpEp->ContextRecord;

		SymSetOptions(SYMOPT_DEFERRED_LOADS);

		if( !SymInitialize(GetCurrentProcess(),0,true) )
			return 0;
		CONTEXT trash= *lpContext;

		DumpCallStack(&trash,true,hProcess);

		return 0;
	}

}

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
	while ( 1 )
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

int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize( NULL );
	unsigned id;
    HANDLE Fhand = (HANDLE)_beginthreadex( NULL , 0 , ThreadFun , NULL , 0 , &id);
	
	WaitForSingleObject( Fhand , INFINITE );
	return 0;
}
