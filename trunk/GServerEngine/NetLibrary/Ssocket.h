/*
*	File  :	 Sscoket.h
*   Brief :  自己封装一个异步的重叠套接字类
*   Author:  Expter 
*	Creat Date:  [2009/11/24]
*/
#pragma once

#include <winsock2.h>

#include "Packet.h"

#pragma  comment( lib ,"WS2_32.lib")

class CSocket 
{
public:
	WORD		m_SendErrorCode;		///  当前错误代码
	WORD		m_SaveCountoRecvPacket; ///  接收到数据包数量

	int			m_ReadIndex;			///  当前包的索引,比如一个数据包含多个包，则表示取除的位置
	int			m_SaveRecvBytes;		///  接收，发送的字节数
	int			m_SaveSendBytes;


public:	
	/*
	 *			网络初始化 
	 */
	CSocket	(	SOCK_TYPE Type = SPECIAL_TYPE,int Size = MAX_SEND_BUFFER_SIZE);

	void		InitSocket( SOCKADDR_IN * addr , SOCKET  sock);

	void		InitOverLapped( OVER_TYPE Type);

	/*
	 *			当前套接字状态设置 
	 */

	void		SetOverReady()	{	m_OverState = READY ;	}

	void		SetOverBad()	{	m_OverState = BAD;		}

	/*
	 *			返回套接字状态
	 */	

	OVER_STATE	GetOverState()	{	return m_OverState;		}

	int			GetSaveCountofRecvPacket()	{	return m_SaveCountoRecvPacket ; }

	int			GetSaveCountofSendPacket()	{	return 0;						}

	/*
	 *  　		遵循Nagle算法
	 *  　		发送，保存，接受数据函数
	 */
	void		SendData(const char * packet ,  int packetSize , bool  bUseN = FALSE );

	void		SaveSendData();

	void		RecvData();

	void		SaveRecvBuffer( int SaveLen);

	/*
	 *	 　		得到异步I/O操作
	 */
	OVERLAPPED	*GetRecvOverLapped()	{	return  &m_RecvOver ; }

	OVERLAPPED	*GetSendOverLapped()	{	return	&m_SendOver	; }

	SOCKADDR_IN *GetSockAddr()			{	return  &m_SockAddr	; }

	SOCKET      *GetSocket()			{	return  &m_Socket	; }

	/*
	 *			检查套接字的状态
	 */
	bool		CheckActiveSocket()		{	return	m_Socket != INVALID_SOCKET ; }

	/*
	 *			得到数据包
	 */ 
	char*		GetRecvBuffer()			{	return m_RecvBuffer ;}

	long		GetPacketHead();	

	char*		GetPacket();		

	/*
	 *			释放资源 
	 */
	virtual     ~CSocket(void);

	void		ReleaseSocket();


private:

	SOCKET		m_Socket;				///  套接字
	SOCK_TYPE	m_SockType;				///  套接字类型
	SOCKADDR_IN	m_SockAddr;				///  连接信息
	DWORD		m_DwFlag;				///  包完整定义
	OVERLAPPED	m_RecvOver;				///  接受操作的异步ＩＯ操作		
	OVERLAPPED	m_SendOver;				///  发送数据包的异步ＩＯ操作
	OVER_STATE	m_OverState;			///  返回套接字状态
	WSABUF		m_WsaRbuf;				///  接收buf,  缓冲区的指针和缓冲区的长度
	WSABUF		m_WsaSbuf;				///  发送buf   缓冲区的指针和缓冲区的长度
	DWORD		m_DwBytesRecv;			///  接收字节
	DWORD		m_DwBytesSent;			///  发送直接
	char		m_RecvBuffer[ BUFFER_SIZE ];/// 接收buf
	char*		m_SendBuffer;			///  发送buf
	int			m_SendBufSize;			///  发送buf大小
};
