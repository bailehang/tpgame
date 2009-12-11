#include "StdAfx.h"
#include "PacketManager.h"

CPacketManager::CPacketManager()
{
	PacketBufferArray_32	= NULL;
	PacketBufferArray_64	= NULL;
	PacketBufferArray_128	= NULL;
	PacketBufferArray_256	= NULL;
	PacketBufferArray_512	= NULL;
	PacketBufferArray_1024	= NULL;
	PacketBufferArray_5120	= NULL;

	m_bUseDosDefence		= TRUE;
}

CPacketManager::~CPacketManager()
{
	sync::scope_guard  guard( m_Section );

	PACKETBUFFER* pBuffer = m_Event_Q.Pop();

	while(pBuffer)
	{
		if(pBuffer->Type== B_T_32)			PacketBufferPool_32.AddNode(pBuffer);
		else if(pBuffer->Type== B_T_64)		PacketBufferPool_64.AddNode(pBuffer);
		else if(pBuffer->Type== B_T_128)	PacketBufferPool_128.AddNode(pBuffer);
		else if(pBuffer->Type== B_T_256)	PacketBufferPool_256.AddNode(pBuffer);
		else if(pBuffer->Type== B_T_512)	PacketBufferPool_512.AddNode(pBuffer);
		else if(pBuffer->Type== B_T_1024)	PacketBufferPool_1024.AddNode(pBuffer);
		else if(pBuffer->Type== B_T_5120)	PacketBufferPool_5120.AddNode(pBuffer);

		pBuffer = m_Event_Q.Pop();
	}

	if(PacketBufferArray_32)
	{
		PacketBufferPool_32.ReleaseList();
		VirtualFree(PacketBufferArray_32,0,MEM_RELEASE);
	}

	if(PacketBufferArray_64)
	{
		PacketBufferPool_64.ReleaseList();
		VirtualFree(PacketBufferArray_64,0,MEM_RELEASE);
	}

	if(PacketBufferArray_128)
	{
		PacketBufferPool_128.ReleaseList();
		VirtualFree(PacketBufferArray_128,0,MEM_RELEASE);
	}

	if(PacketBufferArray_256)
	{
		PacketBufferPool_256.ReleaseList();
		VirtualFree(PacketBufferArray_256,0,MEM_RELEASE);
	}

	if(PacketBufferArray_512)
	{
		PacketBufferPool_512.ReleaseList();
		VirtualFree(PacketBufferArray_512,0,MEM_RELEASE);
	}

	if(PacketBufferArray_1024)
	{
		PacketBufferPool_1024.ReleaseList();
		VirtualFree(PacketBufferArray_1024,0,MEM_RELEASE);
	}

	if(PacketBufferArray_5120)
	{
		PacketBufferPool_5120.ReleaseList();
		VirtualFree(PacketBufferArray_5120,0,MEM_RELEASE);
	}	

}


void  CPacketManager::PacketBufferAlloc()
{
	PacketBufferArray_32	= (PACKETBUFFER_32*)VirtualAlloc(NULL,sizeof(PACKETBUFFER_32)*1000000,MEM_COMMIT,PAGE_READWRITE);
	PacketBufferArray_64	= (PACKETBUFFER_64*)VirtualAlloc(NULL,sizeof(PACKETBUFFER_64)*1000000,MEM_COMMIT,PAGE_READWRITE);
	PacketBufferArray_128	= (PACKETBUFFER_128*)VirtualAlloc(NULL,sizeof(PACKETBUFFER_128)*100000,MEM_COMMIT,PAGE_READWRITE);
	PacketBufferArray_256	= (PACKETBUFFER_256*)VirtualAlloc(NULL,sizeof(PACKETBUFFER_256)*30000,MEM_COMMIT,PAGE_READWRITE);
	PacketBufferArray_512	= (PACKETBUFFER_512*)VirtualAlloc(NULL,sizeof(PACKETBUFFER_512)*30000,MEM_COMMIT,PAGE_READWRITE);
	PacketBufferArray_1024  = (PACKETBUFFER_1024*)VirtualAlloc(NULL,sizeof(PACKETBUFFER_1024)*30000,MEM_COMMIT,PAGE_READWRITE);
	PacketBufferArray_5120  = (PACKETBUFFER_5120*)VirtualAlloc(NULL,sizeof(PACKETBUFFER_5120)*3000,MEM_COMMIT,PAGE_READWRITE);

	for(int i=0;i<1000000;i++)	{ PacketBufferArray_32[i].Type	= B_T_32;	PacketBufferPool_32.AddNode(&PacketBufferArray_32[i]);		}
	for(int i=0;i<1000000;i++)	{ PacketBufferArray_64[i].Type	= B_T_64;	PacketBufferPool_64.AddNode(&PacketBufferArray_64[i]);		}
	for(int i=0;i<100000;i++)	{ PacketBufferArray_128[i].Type = B_T_128;	PacketBufferPool_128.AddNode(&PacketBufferArray_128[i]);	}
	for(int i=0;i<30000;i++)	{ PacketBufferArray_256[i].Type	= B_T_256;	PacketBufferPool_256.AddNode(&PacketBufferArray_256[i]);	}
	for(int i=0;i<30000;i++)	{ PacketBufferArray_512[i].Type	= B_T_512;	PacketBufferPool_512.AddNode(&PacketBufferArray_512[i]);	}
	for(int i=0;i<30000;i++)	{ PacketBufferArray_1024[i].Type= B_T_1024;	PacketBufferPool_1024.AddNode(&PacketBufferArray_1024[i]);	}
	for(int i=0;i<3000;i++)		{ PacketBufferArray_5120[i].Type= B_T_5120;	PacketBufferPool_5120.AddNode(&PacketBufferArray_5120[i]);	}
}

bool CPacketManager::WritePacket(const char* pPacket,long Size,CConnection* pCon)
{
	PACKETBUFFER* pBuffer = NULL;

	if(Size<=32)	 	
	{
		sync::scope_guard  guard( m_Section );
		pBuffer = PacketBufferPool_32.GetHead();
		if(pBuffer) PacketBufferPool_32.RemoveNode(pBuffer);
	}
	else if(Size<=64)
	{
		sync::scope_guard  guard( m_Section );
		pBuffer = PacketBufferPool_64.GetHead();
		if(pBuffer) PacketBufferPool_64.RemoveNode(pBuffer);
	}
	else if(Size<=128)
	{
		sync::scope_guard  guard( m_Section );
		pBuffer = PacketBufferPool_128.GetHead();
		if(pBuffer) PacketBufferPool_128.RemoveNode(pBuffer);
	}
	else if(Size<=256)
	{
		sync::scope_guard  guard( m_Section );
		pBuffer = PacketBufferPool_256.GetHead();
		if(pBuffer) PacketBufferPool_256.RemoveNode(pBuffer);
	}
	else if(Size<=512)
	{
		sync::scope_guard  guard( m_Section );
		pBuffer = PacketBufferPool_512.GetHead();
		if(pBuffer) PacketBufferPool_512.RemoveNode(pBuffer);

	}
	else if(Size<=1024)
	{
		sync::scope_guard  guard( m_Section );
		pBuffer = PacketBufferPool_1024.GetHead();
		if(pBuffer) PacketBufferPool_1024.RemoveNode(pBuffer);
	}
	else if(Size<=5120)
	{
		sync::scope_guard  guard( m_Section );
		pBuffer = PacketBufferPool_5120.GetHead();
		if(pBuffer) PacketBufferPool_5120.RemoveNode(pBuffer);
	}

	if(!pBuffer) return false;

	if( m_bUseDosDefence && pCon ) 
	{
		sync::scope_guard  guard( m_Section );
		pCon->m_SaveCountoRecvPacket++;
	}	

	pBuffer->pCon = pCon;
	memcpy(((PACKETBUFFER_INTERFACE*)pBuffer)->Buffer,pPacket,Size); 

	m_Event_Q.Push(pBuffer);

	return true;
}

PACKETBUFFER* CPacketManager::ReadPacket()
{
	return m_Event_Q.Pop() ;
}

void  CPacketManager::ReadAllPacket(CQueue<PACKETBUFFER> &rTemp)
{
	int size = m_Event_Q.GetSize();
	for ( int i = 0 ; i < size ; i++ )
	{
		rTemp.Push( m_Event_Q.Pop() );
	}
}

void CPacketManager::ReleasePacket(PACKETBUFFER* pBuffer)
{
	if(pBuffer->pCon && pBuffer->pCon->m_SaveCountoRecvPacket) 
	{
		sync::scope_guard  guard( m_Section );
		pBuffer->pCon->m_SaveCountoRecvPacket--;
	}	

	pBuffer->pCon = NULL;

	sync::scope_guard  guard( m_Section );
	if(pBuffer->Type== B_T_32)			PacketBufferPool_32.AddNode(pBuffer);
	else if(pBuffer->Type== B_T_64)		PacketBufferPool_64.AddNode(pBuffer);
	else if(pBuffer->Type== B_T_128)	PacketBufferPool_128.AddNode(pBuffer);
	else if(pBuffer->Type== B_T_256)	PacketBufferPool_256.AddNode(pBuffer);
	else if(pBuffer->Type== B_T_512)	PacketBufferPool_512.AddNode(pBuffer);
	else if(pBuffer->Type== B_T_1024)	PacketBufferPool_1024.AddNode(pBuffer);
	else if(pBuffer->Type== B_T_5120)	PacketBufferPool_5120.AddNode(pBuffer);
}