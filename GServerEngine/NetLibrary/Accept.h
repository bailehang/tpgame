/*
*	File  :  Accept.h
*   Brief :	 一个accept类，多线程管理
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/
#pragma once

#include "Network.h"
#include "..\Public\Thread.h"

class CAccept :
	public CNetwork ,
	public CThread
{

public:
	CAccept(void) {};

	~CAccept(void){};

public:
	/*
	 *	brief:绑定套接字
	 */
	bool			_socket_bind( int port );

	/*
	*   brief:监听连接请求函数
	*/
	unsigned long	_thread_func();

	/*
	 *	brief:关闭请求
	 */
	void			_destroy_accept();
	
private:
	/*
	 *   brief:成员变量
	 */
	SOCKET			m_Sockaccept;		/// 绑定的套接字
	SOCKADDR_IN		m_SockAddr;			/// 端口信息
	HANDLE			m_hThread;			/// 监听线程句柄
};
