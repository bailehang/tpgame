#include "stdafx.h"
#include "BlockPool.h"
#include <algorithm>

/*
void*  HeapPool::Alloc(unsigned long size)
{
	MHIter itr = m_HeapTable.find( size );

	if ( itr != m_HeapTable.end() )
	{
		if( itr->second->m_list.size() > 0)
		{
			void * p = itr->second->m_list.front();
			itr->second->m_list.pop_front();

			itr->second->m_Count ++ ;

			itr->second->m_Used.push_back( p );
			return p;
		}
		else
		{
			void* p = malloc( size );
			itr->second->m_Count ++ ;
			itr->second->m_Used.push_back( p );
			return p;
		}
	}
	
	HeapNode* Node = new HeapNode(size);
	void* p = malloc( size );
	Node->m_Used.push_back( p );
	m_HeapTable.insert( std::pair<long,HeapNode*>( size , Node) );
}

bool   HeapPool::Free( void* MemAddr )
{
	
	return true;
}

bool   HeapPool::Free(void *MemAddr, unsigned long size)
{
	MHIter itr = m_HeapTable.find( size );

	if ( itr != m_HeapTable.end() )
	{
		std::list<void*>::iterator it = find(itr->second->m_Used.begin() ,itr->second->m_Used.end(), MemAddr );
		if ( it != itr->second->m_Used.end() )
		{
			itr->second->m_Used.erase( it );
			itr->second->m_list.push_back( MemAddr );
		}
	}
	return false;
}

void   HeapPool::ReleaseAll()
{

}

*/


BlockPool::BlockPool()
{
	//m_MemPool.clear();
}

BlockPool::~BlockPool()
{
	ReleaseAll() ;
}


void  BlockPool::MemoryInit( Buffer *buf, int idx , int size , eBuff_type type)
{
	m_Buffer[ idx ] =  (buf)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);
	for( int i = 0 ; i < ALLOCSIZE / size ; i++ )
	{
		m_Buffer[idx][i].type = eBT_32;	
		m_MemPool[idx].PushNode(&m_Buffer[idx][i]) ;
	}
}

void  BlockPool::InitMemoryAlloc()
{
	/// 
	long   idx = 32;

	MemoryInit( Buffer32, 0 , sizeof(Buffer32) , eBT_32 );
	MemoryInit( Buffer64, 0 , sizeof(Buffer32) , eBT_32 );
	MemoryInit( Buffer, 0 , sizeof(Buffer32) , eBT_32 );
	MemoryInit( Buffer32, 0 , sizeof(Buffer32) , eBT_32 );
	MemoryInit( Buffer32, 0 , sizeof(Buffer32) , eBT_32 );
	MemoryInit( Buffer32, 0 , sizeof(Buffer32) , eBT_32 );
	MemoryInit( Buffer32, 0 , sizeof(Buffer32) , eBT_32 );

	//m_MemPool = new  CMemory<Buffer>*[ 8] ;sizeof(Buffer32)*ALLOCSIZE/32
// 	m_Buffer[ 0 ] = (Buffer32*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);
// 	m_Buffer[ 1 ] = (Buffer64*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);
// 	m_Buffer[ 2 ] = (Buffer128*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);
// 	m_Buffer[ 3 ] = (Buffer256*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);
// 	m_Buffer[ 4 ] = (Buffer512*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);
// 	m_Buffer[ 5 ] = (Buffer1024*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);
// 	m_Buffer[ 6 ] = (Buffer2048*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);
// 	m_Buffer[ 7 ] = (Buffer5120*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);
// 
// 	for(int i = 0 ; i < ALLOCSIZE / sizeof(Buffer32) ; i++ )
// 	{ m_Buffer[0][i].type = eBT_32;	m_MemPool[0].PushNode(&m_Buffer[0][i]) ;}
 		//m_MemPool[ 0 ] =  new  CMemory<Buffer*>( ALLOCSIZE/32 ,sizeof(Buffer32) );
// 	m_MemPool[ 1 ] =  new  CMemory<Buffer64>( 100 );
// 	m_MemPool[ 2 ] =  new  CMemory<Buffer128>( 100 );
// 	m_MemPool[ 3 ] =  new  CMemory<Buffer256>( 100 );
// 	m_MemPool[ 4 ] =  new  CMemory<Buffer512>( 100 );
// 	m_MemPool[ 5 ] =  new  CMemory<Buffer1024>( 100 );
// 	m_MemPool[ 6 ] =  new  CMemory<Buffer2048>( 100 );
// 	m_MemPool[ 7 ] =  new  CMemory<Buffer5120>( 100 );
// 
// 	CMemory<Buffer32>*   Mem32   = new  CMemory<Buffer32>( 10 );
// 	CMemory<Buffer64>*   Mem64   = new  CMemory<Buffer64>( 10 );
// 	CMemory<Buffer128>*  Mem128  = new  CMemory<Buffer128>(10);
// 	CMemory<Buffer256>*  Mem256  = new  CMemory<Buffer256>(10);
// 	CMemory<Buffer512>*  Mem512  = new  CMemory<Buffer512>(10);
// 	CMemory<Buffer1024>* Mem1024 = new  CMemory<Buffer1024>(10);
// 	CMemory<Buffer2048>* Mem2048 = new  CMemory<Buffer2048>(10);
// 	CMemory<Buffer5120>* Mem5120 = new  CMemory<Buffer5120>(10);

// 	m_MemPool[ eBT_32 ] = Mem32;
// 	m_MemPool[ eBT_64 ] = Mem64;
// 	m_MemPool[ eBT_128] = Mem128;
// 	m_MemPool[ eBT_256] = Mem256;
// 	m_MemPool[ eBT_512] = Mem512;
// 	m_MemPool[ eBT_1024] = Mem1024;
// 	m_MemPool[ eBT_2048] = Mem2048;
// 	m_MemPool[ eBT_5120] = Mem5120;

//	m_MemPool.insert( std::pair<long,CMemory<Buffer*>* >( (long)eBT_32 , new  CMemory<Buffer32*>( ALLOCSIZE/32 ,sizeof(Buffer32)) ));

// 	m_MemPool32.MemoryAlloc( 1000 );
// 	m_MemPool64.MemoryAlloc( 1000 );
// 	m_MemPool128.MemoryAlloc( 1000 );
// 	m_MemPool256.MemoryAlloc( 1000 );
// 	m_MemPool512.MemoryAlloc( 1000 );
// 	m_MemPool1024.MemoryAlloc( 1000 );
// 	m_MemPool2048.MemoryAlloc( 1000 );
// 	m_MemPool5120.MemoryAlloc( 1000 );
}

void*  BlockPool::Alloc( unsigned long size )
{
 	long  index = size/MINBLOCK + (size%MINBLOCK!=0) ? 1 : 0 ;

// 
// 	return static_cast<void*>(m_MemPool[ index ]->GetNewMemoryNode());
//
// 	if ( index == eBT_32)	   return  m_MemPool32.GetNewMemoryNode()->buf;
// 	else if (index == eBT_64 ) return  m_MemPool64.GetNewMemoryNode()->buf;
// 	else if (index == eBT_128) return  m_MemPool128.GetNewMemoryNode()->buf;
// 	else if (index == eBT_256) return  m_MemPool256.GetNewMemoryNode()->buf;
// 	else if (index == eBT_512) return  m_MemPool512.GetNewMemoryNode()->buf;
// 	else if (index == eBT_1024)return  m_MemPool1024.GetNewMemoryNode()->buf;
// 	else if (index == eBT_2048)return  m_MemPool2048.GetNewMemoryNode()->buf;
// 	else if (index == eBT_5120)return  m_MemPool5120.GetNewMemoryNode()->buf;

	return NULL;
}

bool BlockPool::Free(void *Mem)
{
	if( !Mem ) return false;

	Buffer* buf = (Buffer*)( (char*)Mem - offsetof(Buffer32,buf) );
	
	if ( !buf) return false;
	
// 	if ( buf->type == eBT_32)	   return  m_MemPool32.AllocNodeMemory( static_cast<Buffer32*>(buf) );
// 	else if (buf->type == eBT_64 ) return  m_MemPool64.AllocNodeMemory( static_cast<Buffer64*>(buf) );
// 	else if (buf->type == eBT_128) return  m_MemPool128.AllocNodeMemory( static_cast<Buffer128*>(buf) );
// 	else if (buf->type == eBT_256) return  m_MemPool256.AllocNodeMemory( static_cast<Buffer256*>(buf) );
// 	else if (buf->type == eBT_512) return  m_MemPool512.AllocNodeMemory( static_cast<Buffer512*>(buf) );
// 	else if (buf->type == eBT_1024)return  m_MemPool1024.AllocNodeMemory( static_cast<Buffer1024*>(buf) );
// 	else if (buf->type == eBT_2048)return  m_MemPool2048.AllocNodeMemory( static_cast<Buffer2048*>(buf) );
// 	else if (buf->type == eBT_5120)return  m_MemPool5120.AllocNodeMemory( static_cast<Buffer5120*>(buf) );
// 
// 	CMemory<Buffer>*  Cbuf=  m_MemPool[ (long)buf->type ] ;
// 	if ( Cbuf != NULL )
// 	{
// 		return Cbuf->AllocNodeMemory( buf );
// 	}
//
	return false;
}

void   BlockPool::ReleaseAll()
{
// 	for ( int i = 0 ; i < 8 ; i++ )
// 	{
// 		m_MemPool[i]->MemoryRelease();
// 		delete m_MemPool[i];
// 	}
// 	delete m_MemPool[];

// 	m_MemPool32.MemoryRelease( );
// 	m_MemPool64.MemoryRelease( );
// 	m_MemPool128.MemoryRelease( );
// 	m_MemPool256.MemoryRelease( );
// 	m_MemPool512.MemoryRelease( );
// 	m_MemPool1024.MemoryRelease( );
// 	m_MemPool2048.MemoryRelease( );
// 	m_MemPool5120.MemoryRelease( );
}