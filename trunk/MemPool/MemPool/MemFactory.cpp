#include "stdafx.h"
#include "MemFactory.h"


CMemFactory::CMemFactory()
{
	/// æ≤Ã¨≥ı ºªØ
	m_Block.InitMemoryAlloc();
}

char*   CMemFactory::Alloc(unsigned long size)
{
	if( size <= 5120 )
	{
		return m_Block.Alloc( size );
	}
	return  m_Heap.Alloc( size );
}

void    CMemFactory::Free(void* pAddr, unsigned long size)
{
	if( size <= 5120 )
	{
		m_Block.Free( pAddr );
	}
	else
	{
		m_Heap.Free( pAddr );
	}
}