#include "StdAfx.h"
#include "DataBlockPool.h"



CDataBlockPool::CDataBlockPool(long lMaxSize)
{
	m_pBase = new BYTE [lMaxSize];
	if(NULL == m_pBase)
	{
		printf(" CDataBlockPool ·ÖÅäÄÚ´æ³ö´í ");
	}
	m_nMaxSize = lMaxSize;
	m_nCurSize = 0;
}

CDataBlockPool::~CDataBlockPool(void)
{
	if( m_pBase != NULL)
	{
		delete [] m_pBase;
		m_pBase = NULL;
	}
}


void CDataBlockPool::SetCurSize(long lSize)
{
	if( lSize < 0 ) 
		m_nCurSize = 0;
	else if(  lSize > m_nMaxSize )
		m_nCurSize = m_nMaxSize;
	else
		m_nCurSize = lSize;	
}
