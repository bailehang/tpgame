/*
*	File  :	 Queue.h
*   Brief :  一个基于临界区的自定义双向队列
*   Author:  Expter 
*	Creat Date:  [2009/11/2]
*/

#include "locks.h"
#include "CList.h"

template < class T >
class CQueue
{

public:
	CQueue()	
	{
		m_hEvent =  CreateEvent( NULL  , 0  , 0 , NULL );
	}
	~CQueue() 
	{
		CloseHandle( m_hEvent );
	}

	void  SetQEvent()
	{
		SetEvent( m_hEvent );
	}

	HANDLE GetEvent ()	{	return m_hEvent ; }

	int	   GetSize  ()	{	return m_QList.GetSize(); }

	void   Push( T * data) 
	{
		if ( data == NULL )
			return ;
		sync::scope_guard  guard( m_section );

		m_QList.AddNode( data );

	}

	T * Pop()
	{
		sync::scope_guard  guard( m_section );

		if ( m_QList.GetSize() ==  0 )	return NULL;
		
		T * outdata = m_QList.GetHead();
		m_QList.RemoveNode( outdata );

		return outdata;
	}

	void	clear()
	{
		sync::scope_guard  guard( m_section );

		m_QList.ReleaseList();
	}

private:
	
	TLinkedList<T>		 m_QList;
	HANDLE				 m_hEvent;
	sync::csectionlock   m_section;

};