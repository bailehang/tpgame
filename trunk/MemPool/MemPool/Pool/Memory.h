

#pragma  once
#include "List.h"
#include "Locks.h"
#include <list>
#include <vector>
using namespace std;

template  < class T >
class  CMemory : public TlinkedList<T>
{
	typedef  T  Obj;
	typedef  std::vector<Obj>		MemList;
	//typedef  MemList::iterator		MIter;
public:
	CMemory(void)  {  m_MemoryList.clear() ; m_AllocSize = NULL; }
	~CMemory(void) {}


	CMemory( long size ,long  ObjSize)
	{
		m_ObjSize = ObjSize;
		MemoryAlloc( size);
	}
public:
	long		GetAllocMemorySize()	{	return m_AllocSize ; } 

	long		GetFreeMemoryNodeSize() {	return GetSize();  }

	//Obj*		GetMemoryNode(int index){	return m_MemoryArray + index; }

	bool        MemoryAlloc( int size )
	{
		m_AllocSize = size;

		Obj MemoryArray= (Obj)VirtualAlloc( NULL , m_ObjSize*size , MEM_COMMIT , PAGE_READWRITE );
		if(	MemoryArray == NULL )	return false;

		for ( int i = 0 ; i < size ; i++ )
		{
			PushNode( MemoryArray + i );
		}

		m_MemoryList.push_back( MemoryArray );
		return true;
	}

	void        MemoryRelease( )
	{
		sync::scope_guard  sguid ( m_cection );

		if ( m_MemoryList.size() > 0  )
		{
			ReleaseList();
			for ( std::vector<Obj>::iterator it = m_MemoryList.begin() ; it != m_MemoryList.end() ; it++ )
			{
				VirtualFree( *it , 0 , MEM_RELEASE );
			}		
		}
		m_MemoryList.clear();
		m_AllocSize   = 0;
	}

	Obj       GetNewMemoryNode()
	{
		sync::scope_guard  sguid( m_cection );

		Obj  node = GetHead();
		if ( !node )
		{
			if( AppendMemory() )
				return GetNewMemoryNode();

			return NULL;
		}

		RemoveNode( node );

		return node;
	}

	bool  AllocNodeMemory( Obj node)
	{
		if ( !node)
		{
			return false;
		}
		sync::scope_guard  sguid ( m_cection );

		PushNode( node );
		
		return true;
	}

	bool   ReleaseNodeMemory( Obj node)
	{
		if( !node )
		{
			return false;
		}

		sync::scope_guard  sguid( m_cection );

		PushNode( node );

		return true;
	}

	/// append  Memory while free Memory is emptry 
	bool  AppendMemory()
	{
		Obj MemoryArray= (Obj)VirtualAlloc( NULL , m_ObjSize*m_AllocSize , MEM_COMMIT , PAGE_READWRITE );
		if(	MemoryArray == NULL )	return false;

		for ( int i = 0 ; i < m_AllocSize ; i++ )
		{
			PushNode( MemoryArray + i );
		}

		m_AllocSize = m_AllocSize * 2;
		m_MemoryList.push_back( MemoryArray );

		return true;
	}

private:
	long      m_ObjSize;
	long	  m_AllocSize;
	/// 
	MemList   m_MemoryList;
	


	sync::csectionlock  m_cection;
};
