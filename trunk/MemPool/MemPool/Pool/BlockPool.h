



#pragma once 

#include "BaseNode.h"
#include "Memory.h"
#include <map>
using namespace std;

#define  MINBLOCK	  32
#define  INDEXX(IDX)  ( INX/MINBLOCK + IDX%MINBLOCK != 0 ? 1 : 0 )
#define  ALLOCSIZE    50*1024*1024		// 50M


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

class  BlockPool
{
	enum eBuff_Type
	{
		eBT_32 =0,
		eBT_64 ,
		eBT_128,
		eBT_256,
		eBT_512,
		eBT_1024,
		eBT_2048,
		eBT_5120,
	};

	struct  Buffer: CNode<Buffer>
	{
		Buffer( eBuff_Type e ) { type = e ;}
		eBuff_Type   type;
	};

	struct Buffer32 :  Buffer {	char buf[32];	};
	
	struct Buffer64 :  Buffer {	char buf[64];   };

	struct Buffer128 : Buffer {	char buf[128];	};

	struct Buffer256 : Buffer {	char buf[256];  };

	struct Buffer512 : Buffer {	char buf[512];	};

	struct Buffer1024: Buffer {	char buf[1024]; };

	struct Buffer2048: Buffer {	char buf[2048];	};

	struct Buffer5120: Buffer {	char buf[5120]; };

	typedef std::map<long,CMemory<Buffer*>* >   MemPool;
	typedef MemPool::iterator                   Iter;

private:
	//MemPool					m_MemPool;
	Buffer*					m_Buffer[8];
	TlinkedList<Buffer>		m_MemPool[8];
// 	CMemory<Buffer32>		m_MemPool32;
// 	CMemory<Buffer64>		m_MemPool64;
// 	CMemory<Buffer128>		m_MemPool128;
// 	CMemory<Buffer256>		m_MemPool256;
// 	CMemory<Buffer512>		m_MemPool512;
// 	CMemory<Buffer1024>		m_MemPool1024;
// 	CMemory<Buffer2048>		m_MemPool2048;
// 	CMemory<Buffer5120>		m_MemPool5120;

public:
	/// Constructor
	BlockPool() ;
	~BlockPool();
	
	void  InitMemoryAlloc();
		
	void* Alloc( unsigned long size );

	bool  Free( void*  MemAddr);

	void  ReleaseAll();
	

private:
	void   MemoryInit( Buffer *buf, int idx , int size , eBuff_type type);
};