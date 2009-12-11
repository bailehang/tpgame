/*
*	File  :
*   Brief :	 利用内存池的方式管理每一个分配的套接字
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "Connection.h"
#include "..\Public\KeepAlive.h"
#include "..\Public\Memory.h"

class CConnectionManager	
{
public:
	CConnectionManager(void);

	~CConnectionManager(void);

public:

	/*
	 *	 分配连接对象空间
	 */
	bool	InitConnectionManager(int Size,SOCK_TYPE Type,int SendBufferSize,bool bUseKeepAlive,long KeepAliveTimer);

	/*
	 *	 得到最大连接数
	 */
	int		GetMaxConnectionCount()		{ return m_MaxCount;					}
	
	/*
	 *	 得到连接数
	 */
	int		GetActiveConnectionCount()	{ return m_ActiveConnectCount;			}

	/*
	 *	 得到连接数对象池的大小
	 */
	int		GetConnectionPoolCount()	{ return m_ConnectionPool.GetSize();	}

	/*
	 *	 检索index的对象
	 */
	CConnection*	GetConnection(int index); 

	/*
	 *	 分配一个连接对象
	 */
	CConnection*	GetNewConnection(); 

	/*
	 *	 释放某个连接
	 */
	bool			ReleaseConnection(CConnection* pConnection); 

private:

	sync::csectionlock		m_Section;

	int						m_ActiveConnectCount ;

	int						m_MaxCount;

	CMemory<CConnection>	m_ConnectionPool;

	BYTE					*m_ConnectionAccount;

};
