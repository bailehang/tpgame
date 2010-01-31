/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2009/12/14]
*/

#pragma once

#include "TimerList.h"

typedef  CTimerList<CGameEvent>	 Timer;

class CTimer
{
public:

	CTimer(void);

	~CTimer(void);
	
	long		  AddTimer(const void * arg, 
						   unsigned long stime,
						   unsigned long interval);
	long	run();
private:

	Timer	m_Timer;
};
