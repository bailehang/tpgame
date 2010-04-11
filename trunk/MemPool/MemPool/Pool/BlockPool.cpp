#include "stdafx.h"
#include "BlockPool.h"
#include <algorithm>

BlockPool::BlockPool()
{
	//m_MemPool.clear();
}

BlockPool::~BlockPool()
{
	ReleaseAll() ;
}


void  BlockPool::InitMemoryAlloc()
{
	for ( int idx = 0 ; idx < eBT_END ; idx ++ )
	{
		Init( idx , (eBuff_Type) idx );
	}
}

void*  BlockPool::Alloc( unsigned long size )
{
	Buffer* pBuf = NULL;
	long   idx = GetIndex( size );
	if ( idx >=0 )
	{
		 pBuf = m_MemPool[idx].GetHead();
		 if( pBuf != NULL )
		 {
			 m_MemPool[idx].RemoveNode(pBuf);
			 return ((Buffer_Face*)pBuf)->buf;
		 }
		 /// 内存池用完继续分配
		 else
		 {
			std::cout <<" 新增加内存分配 " ;
			Init( idx , (eBuff_Type) idx );   
		 }
	}

	return NULL;
}

bool BlockPool::Free(void *Mem)
{
	if( !Mem ) return false;

	/// 获得偏移地址
	Buffer* buf = (Buffer_Face*)( (char*)Mem - offsetof(Buffer_Face,buf) );
	
	if ( !buf) return false;
	
	m_MemPool[ (long)buf->type ].PushNode(buf);
	  
	return true;
}

void   BlockPool::ReleaseAll()
{
	for ( int i = 0 ; i < eBT_END ; i++ )
	{
		  m_MemPool[i].ReleaseList();
		  std::cout <<" 资源释放 " << typeid(m_listPool[i]).name();
		  for ( std::list<Buffer*>::iterator it = m_listPool[i].begin(); it != m_listPool[i].end(); it ++ )
		  {
			   /// std::cout <<" 资源释放it " << typeid(*it).name() << std::endl;
			   VirtualFree(*it,0,MEM_RELEASE);
		  }  
	}
}