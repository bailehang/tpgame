/*
*	File  :
*   Brief :	 测试服务器类，从Server继承
			 实现Server2个纯虚函数
*   Author:  Expter 
*	Creat Date:  [2009/12/4]
*/
#pragma once

#include "..\Public\TypeDef.h"
#include "..\netlibrary\server.h"

class CServerMsg :
	public CServer
{
public:
	CServerMsg(void);
	~CServerMsg(void);

public:
	void	AccedeProcess( CConnection *pCon , CMessage *pMsg);

	bool	ConnectCacheServer(char* ip,int port);

	void	run( );

public:
	CConnection		m_pCon;
};
