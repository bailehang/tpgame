#include "stdafx.h"
#include "App/Fun.h"
#include "MemFactory.h"

#define  BINDTOFAC(Size,Alloc)\
	m_memfactory->Register(Size, new Alloc(Size,CHUNKSIZE/Size) )

MemFactory::MemFactory()
{
	/// æ≤Ã¨≥ı ºªØ

	m_alloc   = &Chunk::Alloctor;
	m_destory = &Chunk::Free;

	m_memfactory = new  Factory<long,Chunk*,Alloctor,FreeFun>(m_alloc,m_destory);


	for(long i = 0 ; i < sizeof(Range)/ LONGSIZE ; i ++ )
	{
		BINDTOFAC( Range[i] , Chunk  ) ;
	}	  		
}

MemFactory::~MemFactory()
{
	 if ( m_memfactory )
	 {
		 delete m_memfactory;
		 m_memfactory = NULL;
	 }
}


void*   MemFactory::Alloc(unsigned long size)
{
	if( size <= SizeRange )
	{
		int  idx = FindIndx( Range ,0 , sizeof(Range)/ LONGSIZE , size );
		return  m_memfactory->Alloc(idx);
	}
	else
	{
		long range = size  - SizeRange ;
		
		size = ( range / LvlRange +  (range %LvlRange == 0 ? 0 : 1 ) ) * LvlRange + SizeRange;

		if( m_memfactory->CheckExist( size ) )
		{
			return m_memfactory->Alloc( size );
		}
		else
		{
			BINDTOFAC( size , Chunk );
			return m_memfactory->Alloc( size );
		}
	}
}

bool    MemFactory::Free(void* pAddr, unsigned long size)
{
	if ( size == 0 )
	{	
		size = *(long*)( (char*)pAddr - 4 );
	}	
	return m_memfactory->Free( size , pAddr );
}