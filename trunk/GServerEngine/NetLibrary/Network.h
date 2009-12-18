/*
*	File  :	 Network.h
*   Brief :  网络初始化类
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "LibObject.h"

#define  MAX_CONNECTION		1800
#define  KEEPALIVE_TIMER	10000
#define  CONNECTIONMANAGER  CNetwork::ConnectionManager

class CNetwork :
	public CLibObject
{
	
	/*
	 *   Brief:成员变量，为完成端口的全局句柄
	 */
	static  HANDLE	 hCompletionPort;
public:
	
	/*
	 *   Brief: 网络初始化，调用WSAStartup
	 */
	bool	NetworkInit();

	/*
 	 *   Brief:　创建完成端口　
	 */
	void	CreateCompletionPortHandle();

	/*
	 *   Brief:  返回完成端口句柄　
	 */
	static  HANDLE	GetCompletionHandle()	{ return  hCompletionPort ;}

	/*
	 *   Brief: 绑定连接到完成端口上
	 */
	static  bool	SetCompletionPort( SOCKET  sock , CConnection * pCon);


 	CNetwork(void)			{	};

 	virtual ~CNetwork(void) {	};
};
