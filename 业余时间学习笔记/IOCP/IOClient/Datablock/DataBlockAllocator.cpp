#include "StdAfx.h"
#include "DataBlockAllocator.h"
#include "DataBlockPool.h"


CDataBlockAllocator::CDataBlockAllocator(void)
{


}

CDataBlockAllocator::CDataBlockAllocator(int nDBNum, int nDBSize, int bIniShareDB )
:m_nMaxDBNum(nDBNum)
,m_nDBSize(nDBSize)
{
	Initial(bIniShareDB);
	m_MapTest.clear();
}

CDataBlockAllocator::~CDataBlockAllocator(void)
{
	Release();
}

void CDataBlockAllocator::Initial(int bIniShareDB)
{
	int i = 0 ;
	CDataBlockPool  *pDB = NULL;
	for( ; i < m_nMaxDBNum ; i++)
	{
		pDB = new CDataBlockPool(m_nDBSize);
		if(pDB )
		{
			m_FreeDataBlocks.push_back(pDB);
		}
		else
		{
			printf("CDataBlockAllocator 分配出错！\n");
		}
	}

	m_ShareDataBlocks.clear();
	m_AllocedShareDB.clear();

	if(bIniShareDB)
	{
		/// 1M
		long lSize = 1024*1024;
		BYTE *pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));
		pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));
		pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));
		pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));

		/// 2M
		lSize   = 1024*1024*2;
		pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));
		pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));
		pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));

		/// 4M
		lSize   = 1024*1024*4;
		pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));
		pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));

		/// 8M
		lSize   = 1024*1024*8;
		pByte = new BYTE[lSize];
		m_ShareDataBlocks.insert(SHareDBPair(lSize,pByte));
	}
	::InitializeCriticalSection(&m_CSFreeDB);
}

void CDataBlockAllocator::Release()
{
	itFreeDB  it1 = m_FreeDataBlocks.begin();

	CDataBlockPool *pDB = NULL;
	for( ; it1 != m_FreeDataBlocks.end() ; it1 ++ )
	{
		pDB = (*it1);
		if(pDB)
		{
			delete pDB;
			pDB = NULL;
		}
	}
	m_FreeDataBlocks.clear();

	itShareDB it2 = m_ShareDataBlocks.begin();
	for( ; it2 != m_ShareDataBlocks.end(); it2++)
	{
		delete [] (*it2).second;
	}

	::DeleteCriticalSection(&m_CSFreeDB);

	map<long,long>::iterator loit3 = m_MapTest.begin();
	for( ; loit3 != m_MapTest.end(); loit3 ++ )
	{
		printf("CDataBlockAllocator : Flag Release %d %d\n",(*loit3).first , (*loit3).second);
	}
	LeaveCriticalSection(&m_CSFreeDB);
}


CDataBlockPool* CDataBlockAllocator::AllocDB(long lTestFlag )
{
	CDataBlockPool *pData = NULL;
	EnterCriticalSection(&m_CSFreeDB);
	if(this->m_FreeDataBlocks.size() > 0)
	{
		pData = m_FreeDataBlocks.front();
		m_FreeDataBlocks.pop_front();
	}
	else
	{
		pData = new CDataBlockPool(lTestFlag);
		if( pData == NULL)
		{
			printf(" 缓冲区没有足够的内存 ！\n");
		}
	}

	if(pData)
	{
		pData->m_lTestFlag = lTestFlag;
		m_MapTest[lTestFlag] ++;
		printf( " m_MapTest = %d \n", lTestFlag );
	}
	
	if( pData )
	{
		pData->SetCurSize(0);
	}
	LeaveCriticalSection(&m_CSFreeDB);

	return pData;
}

void CDataBlockAllocator::FreeDB(CDataBlockPool * pDB)
{
	if( NULL == pDB)
	{
		return;
	}

	EnterCriticalSection(&m_CSFreeDB);
	
	m_MapTest[pDB->m_lTestFlag] --;
	if((int)m_FreeDataBlocks.size() >= m_nMaxDBNum)
	{
		if(pDB != NULL)
		{
			delete pDB ;
			pDB = NULL;
		}
	}
	else
	{
		m_FreeDataBlocks.push_back(pDB);
	}

	LeaveCriticalSection(&m_CSFreeDB);
}

BYTE * CDataBlockAllocator::AllockShareDb(long lSize)
{
	BYTE *pData = NULL;
	itShareDB itAR = m_ShareDataBlocks.begin();
	for( ; itAR != m_ShareDataBlocks.end() ; itAR ++)
	{
		if(lSize <= (*itAR).first)
		{
			pData = (*itAR).second;
			m_AllocedShareDB[pData] = (*itAR).first;
			m_ShareDataBlocks.erase(itAR);
			break;
		}
	}
	if(!pData)
	{
		printf("AllockShareDb : Get Mem failed ！！\n");
	}
	return pData;
}

void CDataBlockAllocator::FreeShareDB(BYTE* pData)
{
	itAllocR  itAR = m_AllocedShareDB.find(pData);
	if( itAR != m_AllocedShareDB.end() )
	{
		m_ShareDataBlocks.insert(SHareDBPair((*itAR).second,pData));
	}
	else
	{
		delete []pData;
	}
}

void CDataBlockAllocator::PutAllocInfo(const char * pszFileName)
{
	map<long,long>::iterator itTest = m_MapTest.begin();
	for( ; itTest != m_MapTest.end() ; itTest++ )
	{
		if( (*itTest).second != 0 )
		{
			printf("PutAllocInfo : Flag : %d , Count: %d\n",(*itTest).first, (*itTest).second);
		}
	}
}
