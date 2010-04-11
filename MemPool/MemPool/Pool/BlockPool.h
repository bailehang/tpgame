



#pragma once 

#include "BaseNode.h"
#include "List.h"
#include "locks.h"
#include <list>
#include <map>	  
#include <iostream>
using namespace std;

#define  MINBLOCK	  32
#define  INDEXX(IDX)  ( INX/MINBLOCK + IDX%MINBLOCK != 0 ? 1 : 0 )
#define  ALLOCSIZE    30*1024*1024		// 30M

template  <class T>
class  PoolPolicy
{

private:

public:

};


/** 
 *  通过最优查找树管理
 *
 */

class  HeapPool
{
	struct  HeapNode
	{	
		/// memory request rate
		__int64			  m_Count;
		long			  m_Size;
		/// all free memory list
		std::list<void*>  m_list;
		/// all used memory list
		std::list<void*>  m_Used;
		HeapNode( long Size)
		{
			m_Count = 1 ;
			m_Size  = Size ;
			m_list.clear();
		}
	};

	struct  HeapCmp
	{
		bool operator() ( const HeapNode* node1 , const HeapNode* node2 )const
		{
			if ( node1 != node2 )
			{
				/// 如果访问都是一样
				if ( node1->m_Count == node2->m_Count  )
				{
					if ( node1->m_list.size() ==  node2->m_list.size() )
					{
						return node1->m_Size > node2->m_Size;
					}
					else
						return node1->m_list.size() > node2->m_list.size();
				}
				else
					return node1->m_Count    > node2->m_Count;
			}
		}
	};

	/// size -> heapnode 
	typedef std::map<HeapNode*,long ,HeapCmp>  MHEAPLIST;
	typedef MHEAPLIST::iterator				   MHIter;

private:
	MHEAPLIST				m_HeapTable;

public:

	HeapPool(){};
	~HeapPool(){};
		
	void* Alloc( unsigned long size );

	bool  Free( void*  MemAddr);

	bool  Free( void*  MemAddr , unsigned long size );

	void  ReleaseAll();

};

template < typename type_value>
class  Wapper
{
	typedef  type_value  type;

public:
	Wapper( type  _type) : m_type( _type )
	{

	}
	type   TValue()  { return  m_type ;}

private:
	type     m_type;
};

enum eBuff_Type
{
	eBT_32 =0,
	eBT_64,
	eBT_128,
	eBT_256,
	eBT_512,
	eBT_1024,
	eBT_2048,
	eBT_5120,

	eBT_END,
};

struct  Buffer: CNode<Buffer>
{
	eBuff_Type   type;
};

class  BlockPool
{
public:
	
	struct Buffer32 :  Buffer {	char buf[32];	};
	
	struct Buffer64 :  Buffer {	char buf[64];   };

	struct Buffer128 : Buffer {	char buf[128];	};

	struct Buffer256 : Buffer {	char buf[256];  };

	struct Buffer512 : Buffer {	char buf[512];	};

	struct Buffer1024: Buffer {	char buf[1024]; };

	struct Buffer2048: Buffer {	char buf[2048];	};

	struct Buffer5120: Buffer {	char buf[5120]; };

	struct Buffer_Face:Buffer { char buf[5120]; };

private:
	TlinkedList<Buffer>		m_MemPool[eBT_END];
	std::list<Buffer*>		m_listPool[eBT_END];
	Buffer*					m_Buffer[eBT_END];

public:
	BlockPool() ;
	~BlockPool();
	
	void  InitMemoryAlloc();
		
	void* Alloc( unsigned long size );

	bool  Free( void*  MemAddr);

	void  ReleaseAll();
	

private:
	template< typename  Buff>
	void   MemoryInit( long idx , eBuff_Type type)
	{
		m_Buffer[ idx ] =  (Buff*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);

		long  size = ALLOCSIZE / sizeof(Buff) ;
		std::cout << typeid(Buff).name() <<"  Buffer size = " << sizeof(Buff) << " Count = "<< size << std::endl;	
		for( long i = 0 ; i < size ; i++ )
		{
			m_Buffer[idx][i].type = type;	
			m_MemPool[idx].PushNode(&m_Buffer[idx][i]) ;
			m_listPool[idx].push_back( m_Buffer[idx] );
		}
	}

	long  GetIndex(unsigned long size )
	{
		long  arr[ ] = { 32,64,128,256,512,1024,2048,5120 };

		for ( long i = 0; i < eBT_END ; i++ )
		{
			if( (long)size <= arr[i] ) return i ;
		}
		return -1;
	}

	void  Init( long idx , eBuff_Type type)
	{
		if( idx == 0 )     MemoryInit<Buffer32>( idx , type );
		else if( idx == 1) MemoryInit<Buffer64>( idx , type );
		else if( idx == 2) MemoryInit<Buffer128>( idx , type );
		else if( idx == 3) MemoryInit<Buffer256>( idx , type );
		else if( idx == 4) MemoryInit<Buffer512>( idx , type );
		else if( idx == 5) MemoryInit<Buffer1024>( idx , type );
		else if( idx == 6) MemoryInit<Buffer2048>( idx , type );
		else if( idx == 7) MemoryInit<Buffer5120>( idx , type );
	}
};