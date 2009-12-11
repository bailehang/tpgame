/*
*	File  :  AcceptManager.h
*   Brief :  与Accept独立,管理用户请求的Accept的类
			 尽量体现OO的设计思想,低耦合，高类聚
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "Accept.h"

class CAcceptManager
{
	CAccept  Accept;

public:
	CAcceptManager(void) {};

	~CAcceptManager(void){};

	/*
	 *  绑定端口
	 */
	void   CreateAccept( int Port )
	{
		Accept._socket_bind( Port );
		Accept._start_thread();
	};

	/*
	 *  关闭端口
	 */
	void   CloseAccept()	{	Accept._destroy_accept() ; }
};
