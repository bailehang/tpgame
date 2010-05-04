#include "StdAfx.h"
#include "TimerList.h"

template<class type_name>
CTimerList<type_name>::CTimerList(void)
{
// 	m_pTimerArray	= (tagTimerVar*)VirtualAlloc(  NULL , sizeof(tagTimerVar) * MAX_TIMER_NUM , MEM_COMMIT ,PAGE_READWRITE );
// 	
// 	for ( int count = 0 ; count < MAX_TIMER_NUM ; count ++ )
// 	{
// 		m_pTimerArray[i].TimerId = count + 1;
// 		m_pTimerArray[i].Type	 = NULL;
// 		m_pTimerArray[i].StartTime=0;
// 
// 		m_UnUsingList.AddNode( &m_pTimerArray[i] );
// 	}
	m_Count = 0;
}

template<class type_name>
CTimerList<type_name>::~CTimerList(void)
{
// 	m_UnUsingList.ReleaseList();
// 	m_UsingList.ReleaseList();
// 
// 	VirtualFree(m_pTimerArray,0,MEM_RELEASE);
	m_Timerlist.clear();
}


template<class type_name>
long	CTimerList<type_name>::AddTimer(const type_name * arg, 
							 unsigned long stime,
							 unsigned long interval)
{
	sync::scope_guard	sg ( m_section );

	tagTimerVar *  timeNode = new tagTimerVar ;

	if ( timeNode != NULL  )
	{
		//m_UnUsingList.RemoveNode( timeNode );
	
		timeNode->pObj		=	arg;
		timeNode->StartTime	=	stime;
		timeNode->Interval	=   interval;

		m_Timerlist.push( timeNode  );
		
		m_Count ++ ;
		return  m_Count;
	}
	return  0;
}
 
/*
 *	执行定时器的元素
 *	注:这样效率低下，查找需要O(n)
 */
template< class type_name >
long	CTimerList<type_name>::Expired(long CurrentTime)
{
// 	sync::scope_guard	sg ( m_section );
// 
// 	tagTimerVar	* pRoot = m_UsingList.GetHead();
// 
// 	while ( pRoot )
// 	{
// 		if ( (CurrentTime - pRoot->StartTime) >= pRoot->Interval )
// 		{
// 			pRoot->pObj->Execute( pRoot );
// 			
// 			if ( pRoot->Interval != 0 )
// 			{
// 				AddTimer( pRoot->pObj, timeGetTime()+pRoot->Interval , Interval );
// 			}
// 			RemoveTimer( pRoot );
// 			return pRoot->TimerId;
// 		}
// 		pRoot = pRoot->next;
// 	}
	
	while ( !m_Timerlist.empty() )
	{
		tagTimerVar * pt = test.top();
		if ( pt->StartTime > CurrentTime )
			break;
		test.pop();

		// 执行定时器
		pt->pObj->Execute( pt );
		// 循环周期
		if ( pt->Interval> 0 )
		{
			pt->StartTime = time(NULL) + pt->Interval;
			m_Timerlist.push( pt );
		}
		else
		{
			delete pt;
		}
	}
	return 0;
}

template < class  type_name >
long		CTimerList<type_name>::OnTimeOut(const type_name * arg)
{
	return 0;
}

template < class type_name >
bool		CTimerList<type_name>::RelaseGameEndTimer(long id)
{

	sync::scope_guard	sg ( m_section );

// 	tagTimerVar	* pRoot = m_UsingList.GetHead();
// 
// 	while ( pRoot )
// 	{
// 		if ( id == pRoot->TimerId )
// 		{
// 			m_UsingList.RemoveNode ( pRoot );
// 			m_UnUsingList.AddNode( pRoot );
// 			return true ;
// 		}
// 		pRoot = pRoot->next;
// 	}

	return false;
}

template < class type_name >
bool		CTimerList<type_name>::RemoveTimer(tagTimerVar *node)
{
	sync::scope_guard	sg ( m_section );
// 	m_UsingList.RemoveNode ( node );
// 	m_UnUsingList.AddNode( node );

	return true;
}
