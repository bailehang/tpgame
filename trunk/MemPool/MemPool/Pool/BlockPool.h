/** 
 *  file   BlockPool.h
 *  brief  采用静态分配的封装类,做好策略，把所有的静态分配大小全部预分配。
 *         然后根据需要分配大小进行预分配。可以支持再分配策略..
 *         复杂度:释放，分配 O(1)
 *  author Expter
 *  date   2010/04/12
 */
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
#define  ALLOCSIZE    20*1024*1024		// 40M


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

/** 
 *  性能分析:
 *     分配 0(1),释放O(1),效率是常量的
 */

/// 做策略的内存分配
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
		
	char* Alloc( unsigned long size );

	bool  Free( void*  MemAddr);

	void  ReleaseAll();
	

private:
	template< typename  Buff>
	bool   MemoryInit( long idx , eBuff_Type type)
	{
		m_Buffer[ idx ] =  (Buff*)VirtualAlloc(NULL,ALLOCSIZE,MEM_COMMIT,PAGE_READWRITE);

		long  ObjSize = sizeof(Buff);
		long  size = ALLOCSIZE / ObjSize;
		for( long i = 0 ; i < size ; i++ )
		{
			((Buff*)m_Buffer[idx]+i)->type = type;	
			m_MemPool[idx].PushNode((Buff*)m_Buffer[idx]+i) ;
		}
#ifdef  _DEBUG
		std::cout << typeid(Buff).name()<<"Bsize="<< sizeof(Buff) << "Count ="<< size 
				  <<"size ="<<m_MemPool[idx].GetSize()<< std::endl;	
#endif
		m_listPool[idx].push_back( m_Buffer[idx] );
		return true;
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

	bool  Init( long idx , eBuff_Type type)
	{
		if( idx == 0 )     return MemoryInit<Buffer32>( idx , type );
		else if( idx == 1) return MemoryInit<Buffer64>( idx , type );
		else if( idx == 2) return MemoryInit<Buffer128>( idx , type );
		else if( idx == 3) return MemoryInit<Buffer256>( idx , type );
		else if( idx == 4) return MemoryInit<Buffer512>( idx , type );
		else if( idx == 5) return MemoryInit<Buffer1024>( idx , type );
		else if( idx == 6) return MemoryInit<Buffer2048>( idx , type );
		else if( idx == 7) return MemoryInit<Buffer5120>( idx , type );
		return true;
	}
};