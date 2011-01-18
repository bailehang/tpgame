#include "stdafx.h"
#include "FreeList.h"

/// @param blocksize Ã¿¿é´óÐ¡
List::List( long blocksize, long  count)
{
	m_blockcount = count;
	m_nodesize = blocksize;
	m_validsize= count;
	m_firstblock= 0;

	Init();
}

List::~List()			
{ 
	ReleaseAll();
}

void*   List::Allocator()
{
	if( m_validsize <= 0 ) return NULL;

	char *pRet = m_pData + ( m_firstblock * BLOCKSIZE(m_nodesize) );

	m_firstblock = *(long*)pRet ;

	--m_validsize;

	*(long*)pRet = m_nodesize;

	return pRet + LONGSIZE ;
}

bool    List::Free(void *pData) 
{
	char *pList = (char*)pData - LONGSIZE;

	*(long*)pList = m_firstblock;

	m_firstblock = ( pList - m_pData ) / BLOCKSIZE(m_nodesize);

	++ m_validsize ;

	return true;
}

bool	List::Check(void* pData) const 
{
	if ( !pData || pData < m_pData )  return false;

	char *pList = (char*)pData - LONGSIZE;

	if ( (pList-m_pData) %  BLOCKSIZE(m_nodesize) != 0 ||
		(pList-m_pData) /  BLOCKSIZE(m_nodesize) > (unsigned long)m_blockcount )
		return false;

	return true;
}

void    List::Init()
{
	long  size = BLOCKSIZE(m_nodesize) * m_blockcount ;
	m_pData = (char*) VirtualAlloc(NULL,size,MEM_COMMIT,PAGE_READWRITE);

	assert( m_pData );

	char* pData = m_pData ;
	for ( long i = 0 ; i < m_blockcount ; i++ )
	{
		*( long * )pData = (i+1);
		pData += BLOCKSIZE(m_nodesize) ;
	}
}

void    List::ReleaseAll()
{
	if ( m_pData )
	{
		VirtualFree( m_pData , 0 , MEM_RELEASE );
		m_pData = NULL;
		m_blockcount = m_nodesize = m_validsize = m_firstblock = 0;
	}
}