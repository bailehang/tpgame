/*
*	File  :	 Connection.h
*   Brief :	 继承Socket,表示每一个与连接，主要处理每个套接字数据包的逻辑上的操作，比如封包，解包
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/
#pragma once

#include "ssocket.h"
#include "..\Public\locks.h"
#include "..\Public\CList.h"
#include "..\Public\BaseNode.h"

class CConnection :
	public CSocket,
	public CBaseNode<CConnection> 
{	

public:
	/*
	 *	 初始化，构造函数
	 */
	CConnection(void):CSocket(){	}

	CConnection(SOCK_TYPE Type , int BufSize) : CSocket( Type , BufSize )	{	}

	void	Init( SOCKADDR_IN * addr , SOCKET sock );

	/*
	 *		Get操作 
	 */
	char*	GetIP() { return m_ip; }

	LPVOID	GetClientInfo() { return m_ClientInfo;}

	long	GetPacketRecvTime() { return m_RecvTime; }

	/*
	 *		返回一个数据流的数据包
	 */
	char*   GetSamplePacket(long RestBytes,long* PacketSize,long* ErrorCode);

	/*
	 *		设置操作 
	 */
	void	SetClientInfo(LPVOID Info) { m_ClientInfo = Info;}

	void	SetUID(long ID)			   { m_uid = ID;	   	 }

	long	GetUID()				   { return m_uid;		 }

	void	SetWait()				   { m_State = false;	 }
 
	void	SetActive()				   { m_State = true;	 }

	void	SetPacketRecvTime(long time){ m_RecvTime = time;}

	/*
	 *		包的操作 
	 */
	bool	QesActive()				   { return		m_State; }


	/*
	 *		发送和接收操作
	 */
	void	SendPacket(CMessage* packet,bool bUseNegleAlgorithm = false);

	void	RecvPacket();

	/*
	 *		释放操作
	 */
	virtual	~CConnection(void);

	void	Release();

private:
	char	m_ip[16];				/// 当前套接字的ip	
	long	m_uid;					/// uid
	bool	m_State;				/// 状态
	BYTE	m_DeleyCount;	
	LPVOID	m_ClientInfo;
	long	m_RecvTime;
};