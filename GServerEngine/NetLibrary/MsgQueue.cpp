#include "StdAfx.h"
#include "MsgQueue.h"

CMsgQueue::CMsgQueue(void)
{
	m_msgQueue.clear();
}

CMsgQueue::~CMsgQueue(void)
{
	m_msgQueue.clear();
}


bool	CMsgQueue::PushMessage(PACKETBUFFER *pMsg)
{
	m_msgQueue.Push( pMsg );
	
	return true;
}

PACKETBUFFER *	CMsgQueue::PopMessage()
{
	return  m_msgQueue.Pop();
}

void	CMsgQueue::GetSizeMessage(CMsgQueue::msgQueue &pTempQMsg, int size)
{
	int  lsize = GetSize();
	
	if( lsize >= size )		lsize	=	size;

	for ( int i = 0 ; i < lsize ; i++ )
	{
		pTempQMsg.Push( m_msgQueue.Pop() );
	}
	
}

void	CMsgQueue::GetAllMessage(CMsgQueue::msgQueue &pTempQMsg)
{
	int  lsize = GetSize();

	for ( int i = 0 ; i < lsize ; i++ )
	{
		pTempQMsg.Push( m_msgQueue.Pop() );
	}
}

void	CMsgQueue::Clear()
{
	m_msgQueue.clear(	);
}

int 	CMsgQueue::GetSize()
{
	return  m_msgQueue.GetSize();
}