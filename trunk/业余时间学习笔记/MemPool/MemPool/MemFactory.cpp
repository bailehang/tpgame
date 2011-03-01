#include "stdafx.h"
#include "MemFactory.h"

const long  blocksize = 5120;

#define  BINDTOFAC(Size,Alloc)\
	m_factory->Add(Size,new Alloc(Size,AALLOCSIZE/Size) )


CMemFactory::CMemFactory()
{
	/// æ≤Ã¨≥ı ºªØ

	m_alloc   = &SmallObjAllocator::Allocate;
	m_destory = &SmallObjAllocator::Destory;

	m_factory = new  Factory<long,SmallObjAllocator*,AllocMem,Destory>(m_alloc,m_destory);

	BINDTOFAC( 16 , SmallObjAllocator );
	BINDTOFAC( 32 , SmallObjAllocator );
	BINDTOFAC( 64 , SmallObjAllocator );
	BINDTOFAC( 128 , SmallObjAllocator );
	BINDTOFAC( 256 , SmallObjAllocator );
	BINDTOFAC( 512 , SmallObjAllocator );
	BINDTOFAC( 1024 , SmallObjAllocator );
	BINDTOFAC( 2048 , SmallObjAllocator );
	BINDTOFAC( 5120 , SmallObjAllocator );
}	

int     CMemFactory::Index(unsigned long size)
{
		unsigned long  arr[ ] = { 32,64,128,256,512,1024,2048,5120 };

		return arr[ FindIndx( arr , 0 , sizeof(arr)/sizeof(unsigned long) , size ) ];
}

void*   CMemFactory::Alloc(unsigned long size)
{
	if( size <= blocksize )
	{
		long idx = Index(size);
		return  m_factory->Alloc(idx);
	}
	else
	{
		long range = size  - 5120 ;
		
		size = ( range / 512 +  (range %512 == 0 ? 0 : 1 ) ) * 512 + 5120;

		if( m_factory->CheckExist( size ) )
		{
			return m_factory->Alloc( size );
		}
		else
		{
		   BINDTOFAC( size , SmallObjAllocator );
		   return m_factory->Alloc( size );
		}
	}
}

void    CMemFactory::Free(void* pAddr, unsigned long size)
{
	if ( size == 0 )
	{	
		size = *(long*)( (char*)pAddr - 4 );
	}	
	m_factory->Release( size , pAddr );
}
