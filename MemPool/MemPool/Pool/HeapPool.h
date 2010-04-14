/** 
 *  file   list.h
 *  brief  采用堆分配的封装类,这里把分配的同大小内存的用HeapNode保存
 *         每一个HeapNode里面有2个list,每一个list里面有多个结点，
		   每一个节点的ptr指向动态分配的内存大小，ptr指向的前8个字节为每个节点的地址值。
		   我们释放的时候直接传入地址可以得到节点指针.
		   复杂度:释放，分配 O(1)
 *  author Expter
 *  date   2010/01/12
 */
#pragma  once 

#include "BaseNode.h"
#include "List.h"
#include "locks.h"
#include <list>
#include <map>	  
#include <iostream>
using namespace std;

class  HeapPool
{

	struct  listNode : CNode<listNode>
	{
		long  size;
		/// 指向的内存
		char* ptr;
	};
	struct  HeapNode
	{	
		/// memory request rate
		__int64			  m_Count;
		/// all free memory list
		TlinkedList<listNode> m_FreeList;
		/// all used memory list
		TlinkedList<listNode> m_Used;
		HeapNode( )
		{
			m_Count = 0 ;
			m_FreeList.ReleaseList();
			m_Used.ReleaseList();
		}
	};

	typedef std::map<unsigned long,HeapNode* >	MHEAPLIST;
	typedef MHEAPLIST::iterator					MHIter;

private:
	MHEAPLIST				m_HeapTable;

public:

	HeapPool();
	~HeapPool();

	///  请求内存分配
	char* Alloc( unsigned long size );

	///   释放内存
	bool  Free( void*  MemAddr);

	///   释放内存，带大小的
	bool  Free( void*  MemAddr , unsigned long size );

	///   清理分配资源 
	void  ReleaseAll();

private:
	
	/// 内存分配
	char*  MemorySize( unsigned long size )
	{
		MHIter it = m_HeapTable.find( size );
		/// 找到映射表
		if ( it != m_HeapTable.end() )
		{
			listNode* node = new listNode;
			node->size = size;
			node->ptr  = new char[ size + 8];
			it->second->m_Used.PushNode( node );
			it->second->m_Count ++ ;
			memcpy(node->ptr ,&node , 8 );  // 前8個地址指向的節點地址
			return node->ptr+8;
		}
		else
		{
			HeapNode* pHeapNode = new HeapNode;
			listNode* node = new listNode;
			node->size = size;
			node->ptr  = new char[ size + 8];
			memcpy(node->ptr ,&node , 8 );
			pHeapNode->m_Used.PushNode( node );
			pHeapNode->m_Count ++ ;
			m_HeapTable.insert( std::pair<long,HeapNode*>(size,pHeapNode) );
			return node->ptr+8;
		}
		return NULL;
	}

	/// 清理策略
	void   Clear( HeapNode* Heap )
	{
		long  FreeSize = Heap->m_FreeList.GetSize();
		long  UsedSize = Heap->m_Used.GetSize();

		if (  UsedSize <= FreeSize || FreeSize>= 100 )
		{
			for ( long i = 0 ; i < FreeSize/2 ; i++ )
			{
				listNode* node = Heap->m_FreeList.GetHead();
				Heap->m_FreeList.RemoveNode( node );
				delete node->ptr;
				delete node;
			}
		}
	}
};