/*
*	File  :	 iocp.h
*   Brief :	 完成端口工作线程
*   Author:  Expter 
*	Creat Date:  [2009/11/24]
*/

#pragma once

#include <process.h>

#include "Network.h"
#include "..\Public\locks.h"
#include "..\Public\CList.h"
#include "..\Public\Memory.h"

#define MAX_WORKER_COUNT	8
#define WORKER_DESTROY_KEY	1 


class CIOCP: public CNetwork
{

private:

	void StartWorkerThread();
	void DestroyWorker();

public:
	CIOCP()			 {};
	
	virtual ~CIOCP() {};

	/*
	 *		开始iocp
	 */
	void	StartIOCP();	

	/*
	 *		停止iocp
	 */
	void	StopIOCP();

	/*
	 *		设置工作事件
	 */
	void	SetWorkerEvent();

	/*
	 *		连接错误
	 */
	void	ConnectionError(CConnection* pCon);

	/*
	 *		完成端口工作线程
	 */
	static unsigned int __stdcall WINAPI _work_thread_(LPVOID pVoid);

private:
	sync::csectionlock   m_Scection;
	int					 m_WorkerNum;
	int					 m_HandleIndex;
	HANDLE				 m_hEvent[10];
};