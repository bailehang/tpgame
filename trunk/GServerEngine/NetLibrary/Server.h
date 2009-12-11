/*
*	File  :
*   Brief :	 一个服务器管理类，采用iocp的网络封装接口
			 一个抽象类，提供一个接受消息处理接口run
			 提供网络监听和主动连接的功能........
*   Author:  Expter 
*	Creat Date:  [2009/12/4]
*/

#pragma once

#include "PacketManager.h"
#include "AcceptManager.h"
#include "Connection.h"
#include "iocp.h"
#include "MsgQueue.h"

class CServer :
	public CNetwork ,
	public CThread 
{
public:
	CServer(void)	{};
	~CServer(void)	{};

public:
	/*
	 *			ip设置
	 */
	void		GetServerNameAndIP(  char * host , char *ip );

	/*
	 *			初始化，开启网络完成端口
	 */
	void		Read( int MaxConnection		 = MAX_CONNECTION,	 SOCK_TYPE Type = GENERAL_TYPE ,
					  int SockSendBufferSize = SEND_BUFFER_SIZE, bool bUseKeepAliveOption = TRUE,
					  bool bUseDosDefenceOption = true);

	/*
	 *			建立服务器端口映射，开启端口绑定
	 */
	void		Start( char * ServerName , int ServerPort );

	/*
	 *			结束网络
	 */
	void		Stop();

	/*
	 *			主动连接接口，返回成功失败
	 */
	bool		Connect(CConnection* pCon,char* ip, int port, long uid = CONNECTOR_UID);

	/*
	 *			关闭连接
	 */
	bool		DisConnect( CConnection *pCon );


	PACKETBUFFER_INTERFACE	* GetBuffer();

	/*
	 *			释放资源
	 */
	void		ReleaseBuffer( PACKETBUFFER_INTERFACE * pPacketBuffer );

	bool		ReleaseConnection( CConnection * pCon);
	
	bool		ReleaseClient(CConnection* pCon) ;

	/*
	 *			处理每个连接的缓存数据线程
	 */
	unsigned	long  _thread_func();

	/*
	 *			发送每个连接的未发送数据
	 */
	void		SendProcess(  long	NowTime );

	/*
	 *			是否进行ddos防护，主要是统计每个连接发包数量
	 */
	bool		DefenceDosAttack( CConnection *pCon);

	void		PacketProcess(PACKETBUFFER_INTERFACE* pPacketBuffer);


	/*
	 *			消息处理函数
	 */
	virtual		void	AccedeProcess( CConnection *pCon , CMessage *pPacket)=0;

	/*
	 *			消息的主循环，提取缓冲区的消息
	 */

	virtual		void run() = 0;

protected:
	CIOCP				 m_Iocp;
	CAcceptManager		 m_AcceptManager;
	CMsgQueue			 m_MsgQueue;				// 接收到的数据包队列
	sync::csectionlock   m_Section;
};
