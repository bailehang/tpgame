/**
 * Brief :  定义一个线程基类
 * Author:  Expter  
 * Date  :  [2009-11-1]                           
 */

#pragma  once

#include <windows.h>
#include <process.h>

class CThread
{

protected:
	bool     bThreadCreate;
	HANDLE   hThread;

public:
	/*
	 *   线程开始	
	 */
	virtual void _start_thread(void)
	{
		bThreadCreate = true;

		hThread = (HANDLE)_beginthreadex( NULL , 0 , _thread_proc , this  , 0 , 0 );

	}

	/*
	 *	终止线程	
	 */
	virtual void _stop_thread(void)
	{
		unsigned long cDestroy;
		bThreadCreate = false; 
		for ( ;; )
		{	 
			GetExitCodeThread( hThread , &cDestroy );
			if ( cDestroy != STILL_ACTIVE )   break;
		}

		CloseHandle(  hThread );

		_endthreadex( NULL );
	}

	/*
	 *	检测线程运行状态	
	 */
	virtual bool _check_thread_live(void)	{	return bThreadCreate; }

	/*
	 *	线程调用函数
	 */
	static  unsigned int __stdcall WINAPI  _thread_proc( LPVOID pVoid)
	{
		CThread *pThis = (CThread*)pVoid;

		return  pThis->_thread_func() ;
	}

	/*
	 *	线程处理函数
	 */
	virtual unsigned long  _thread_func() = 0 ;

};
