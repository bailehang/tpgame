/*
*	File  :	Memory.h
*   Brief :  一个针对对象内存池的管理，主要是通过链表来管理内存。
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "List.h"
#include "locks.h"

template < class obj , long  ObjSize>
class CMemory  : public TLinkedList<obj>
{
public:

	///
	///  Brief:构造函数初始化 
	///
	CMemory(void)	{	MemoryArray = NULL ; AllocSize = 0 ; }	

	~CMemory(void)	{	}

	///
	///  Brief: 操作管理函数
	/// 
	int		GetAllocMemorySize()	{	return AllocSize ; } 

	int		GetFreeMemoryNodeSize() {	return GetSize();  }

	obj *	GetMemoryNode(int index){	return MemoryArray + index; }

	///  Brief: 内存分配
	bool	MemoryAlloc( int size ) 
	{
		AllocSize  = size;

		MemoryArray= (obj*)VirtualAlloc( NULL , sizeof(obj)*size , MEM_COMMIT , PAGE_READWRITE );
		if(	MemoryArray == NULL )	return false;

		for ( int i = 0 ; i < size ; i++ )
		{
			AddNode( MemoryArray + i );
		}
		return true;
	}

	void	MemoryRelease()
	{
		sync::scope_guard  sguid( m_cection );

		if ( MemoryArray)
		{
			ReleaseList();
			VirtualFree( MemoryArray , 0 , MEM_RELEASE );
		}

		MemoryArray = NULL;
		AllocSize	= 0;
	}

	obj * GetNewMemoryNode()
	{	
		sync::scope_guard  sguid( m_cection );

		obj * node = GetHead();
		if ( !node)
		{
			return NULL;
		}

		RemoveNode( node );

		return node ;
	}

	bool  AllocNodeMemory( obj * node)
	{
		if ( !node )	return false;

		sync::scope_guard  sguid( m_cection );
	
		RemoveNode( node );
		
		return true;
	}

	bool  ReleaseNodeMemory( obj * node)
	{
		if ( !node )	return false;

		sync::scope_guard  sguid( m_cection );

		AddNode( node );

		return true;
	}

private:
	obj * MemoryArray;
	int	  AllocSize;
	sync::csectionlock  m_cection;

};
