

#pragma  once
#include "List.h"
#include "Locks.h"

template  < class T >
class  CMemory : public TlinkedList<T>
{
	typedef  T  Obj;

public:
	CMemory(void)  {  m_MemoryArray = NULL ; m_AllocSize = NULL; }
	~CMemory(void) {}


public:
	long		GetAllocMemorySize()	{	return m_AllocSize ; } 

	long		GetFreeMemoryNodeSize() {	return GetSize();  }

	Obj*		GetMemoryNode(int index){	return m_MemoryArray + index; }

	bool        MemoryAlloc( int size)
	{
		m_AllocSize = size;

		m_MemoryArray= (Obj*)VirtualAlloc( NULL , sizeof(Obj)*size , MEM_COMMIT , PAGE_READWRITE );
		if(	m_MemoryArray == NULL )	return false;

		for ( int i = 0 ; i < size ; i++ )
		{
			AddNode( m_MemoryArray + i );
		}
		return true;
	}

	void        MemoryRelease( )
	{
		sync::scope_guard  sguid ( m_cection );

		if (  m_MemoryArray != NULL )
		{
			ReleaseList();
			VirtualFree( m_MemoryArray , 0 , MEM_RELEASE );
		}

		m_MemoryArray = NULL;
		m_AllocSize   = 0;
	}

	Obj*        GetNewMemoryNode()
	{
		sync::scope_guard  sguid( m_cection );

		Obj*  node = GetHead();
		if ( !node )
		{
			return NULL;
		}

		RemoveNode( node );

		return node;
	}

	bool  AllocNodeMemory( Obj* node)
	{
		if ( !node)
		{
			return false;
		}
		sync::scope_guard  sguid ( m_cection );

		RemoveNode( node );
		
		return true;
	}

	bool   ReleaseNodeMemory( Obj* node)
	{
		if( !node )
		{
			return false;
		}

		sync::scope_guard  sguid( m_cection );

		AddNode( node );

		return true;
	}

private:
	/// 
	Obj*   m_MemoryArray;
	long   m_AllocSize;

	sync::csectionlock  m_cection;
};
