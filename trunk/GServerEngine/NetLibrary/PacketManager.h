/*
*	File  :  PacketManager.h
*   Brief :	 通过Queue来管理所有的数据  (一个内存管理方式)
			 一种内存池的实现，参考loki的小对象管理方式设计
*   Author:  Expter 
*	Creat Date:  [2009/12/9]
*/
#pragma once

#include "Packet.h"
#include "Connection.h"
#include "..\Public\Queue.h"

/*
 *	 数据包
 */
struct PACKETBUFFER : CBaseNode<PACKETBUFFER>
{
	CConnection*	pCon;
	BUFFER_TYPE		Type;
};

/*
 *	 不同大小的数据包定义
 */
struct PACKETBUFFER_32		: PACKETBUFFER	{	char Buffer[32];	};
struct PACKETBUFFER_64		: PACKETBUFFER	{	char Buffer[64];	};
struct PACKETBUFFER_128		: PACKETBUFFER	{	char Buffer[128];	};
struct PACKETBUFFER_256		: PACKETBUFFER	{	char Buffer[256];	};
struct PACKETBUFFER_512		: PACKETBUFFER	{	char Buffer[512];	};
struct PACKETBUFFER_1024	: PACKETBUFFER	{	char Buffer[1024];	};
struct PACKETBUFFER_5120	: PACKETBUFFER	{	char Buffer[5120];	};
struct PACKETBUFFER_INTERFACE: PACKETBUFFER	{	char Buffer[5120];	};


class CPacketManager
{
public:
	/*
	 *	  构造函数，初始化
	 */
	CPacketManager();

	/*
	 *	  析构函数
	 */
	~CPacketManager();

	/*
	 *	  是否限制Dos攻击
	 */
	void	SetDosDefenceOption( bool bUse)	{ m_bUseDosDefence = bUse ;}

	/*
	 *	  分配内存
	 */
	void	PacketBufferAlloc();

	/*
	 *	  数据包操作
	 */
	void	SetPacketEvent()	{	m_Event_Q.SetQEvent();	}

	HANDLE	GetPacketEvent()	{	return m_Event_Q.GetEvent() ; }

	bool    WritePacket(const  char* pPacket,long Size,CConnection* pCon);

	PACKETBUFFER * ReadPacket( );

	void	ReadAllPacket( CQueue<PACKETBUFFER> &rTemp);

	/*
	 *		释放空间
	 */
	void    ReleasePacket( PACKETBUFFER *pBuffer);

private:
	/*
	 *	   各个大小的数据包
	 */
	PACKETBUFFER_32*			PacketBufferArray_32;
	PACKETBUFFER_64*			PacketBufferArray_64;
	PACKETBUFFER_128*			PacketBufferArray_128;
	PACKETBUFFER_256*			PacketBufferArray_256;
	PACKETBUFFER_512*			PacketBufferArray_512;
	PACKETBUFFER_1024*			PacketBufferArray_1024;
	PACKETBUFFER_5120*			PacketBufferArray_5120;

	/*
	 *	通过链表的方式管理这些空闲的内存数据包
	 */
	TLinkedList<PACKETBUFFER>	PacketBufferPool_32;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_64;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_128;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_256;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_512;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_1024;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_5120;

	/*
	 *	接收到的数据包
	 */
	CQueue<PACKETBUFFER>		m_Event_Q;

	bool						m_bUseDosDefence;

	sync::csectionlock			m_Section;
};