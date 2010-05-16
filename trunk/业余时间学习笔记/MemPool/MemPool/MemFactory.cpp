#include "stdafx.h"
#include "MemFactory.h"

#define  AALLOCSIZE    20*1024*1024		// 20M

CMemFactory::CMemFactory()
{
	/// æ≤Ã¨≥ı ºªØ
	//m_Block.InitMemoryAlloc();
	AllocMem  alloc   = &SmallObjAllocator::Allocate;
	Destory   destory = &SmallObjAllocator::Destory;
	m_factory = new  Factory<long,SmallObjAllocator*,AllocMem,Destory>(alloc,destory);

	m_factory->Add( eBT_32,  new SmallObjAllocator(32,  AALLOCSIZE/32) );
	m_factory->Add( eBT_64,  new SmallObjAllocator(64,  AALLOCSIZE/64) );
	m_factory->Add( eBT_128, new SmallObjAllocator(128, AALLOCSIZE/128) );
	m_factory->Add( eBT_256, new SmallObjAllocator(256, AALLOCSIZE/256) );
	m_factory->Add( eBT_512, new SmallObjAllocator(512, AALLOCSIZE/512) );
	m_factory->Add( eBT_1024, new SmallObjAllocator(1024, AALLOCSIZE/1024) );
	m_factory->Add( eBT_2048, new SmallObjAllocator(2048, AALLOCSIZE/2048) );
	m_factory->Add( eBT_5120, new SmallObjAllocator(5120, AALLOCSIZE/5120) );
}	

int     CMemFactory::Index(unsigned long size)
{
	long  arr[ ] = { 32,64,128,256,512,1024,2048,5120 };

	for ( long i = 0; i < eBT_END ; i++ )
	{
		if( (long)size <= arr[i] ) return i ;
	}
	return -1;
	
}

void*   CMemFactory::Alloc(unsigned long size)
{
	if( size <= 5120 )
	{
		long idx = Index(size);
		return  m_factory->Alloc(idx);
		//return m_Block.Alloc( size );
	}
	else
	{
		char *p = new char[size];
		m_setList.insert(p);
		return p;
	}
}

void    CMemFactory::Free(void* pAddr, unsigned long size)
{
	if( size <= 5120 )
	{
		long idx = Index(size);
		m_factory->Release( idx ,pAddr );
	}
	else
	{
		if( pAddr )
		{
			m_setList.erase(pAddr);
			delete pAddr;
			pAddr = NULL;
		}
	}
}