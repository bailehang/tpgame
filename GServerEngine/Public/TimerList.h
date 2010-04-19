/*
*	File  :
*   Brief :	 ��ʱ����������,δ���������Ż�
			 ע��δ�����ҽ���ϵ��Ż��������ܴ��ʱ��Ҫ������ȫ������ʱ��Ƭ�ϲ�׼ȷ
*   Author:  Expter 
*	Creat Date:  [2009/12/14]
*/

#pragma once
#include "GameEvent.h"
#include "BaseNode.h"
#include "List.h"
#include "locks.h"

#define		MAX_TIMER_NUM		2048

/*
 *	 Define	TimerType
 */
enum    TimerType
{
	ObjType_Player	= 0x01,
	ObjType_Pet,
	ObjType_Monster,
};

/*
 *	 Define Timer Node	with list connect together
 */
struct  tagTimerVar	 : CBaseNode<tagTimerVar>
{
	tagTimerVar(){};

	long		StartTime;
	long		Interval;
	long		TimerId;
	TimerType	Type;
	void*		pObj;
};

template< class  type_name>
class CTimerList
{
public:
	CTimerList(void);


	~CTimerList(void);

	long		  AddTimer(const type_name * arg, 
						   unsigned long stime,
						   unsigned long interval);

	long		  Expired( long  CurrentTime = 0);
	
	long		  OnTimeOut( const type_name * arg);

	bool		  RelaseGameEndTimer( long  id );

	bool		  RemoveTimer( tagTimerVar * node);


private:
	tagTimerVar*				m_pTimerArray;
	TLinkedList<tagTimerVar>	m_UnUsingList;
	TLinkedList<tagTimerVar>	m_UsingList;
	sync::csectionlock			m_section;
};