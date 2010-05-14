#include "StdAfx.h"
#include "Timer.h"

CTimer::CTimer(void)
{
}

CTimer::~CTimer(void)
{
}

long   CTimer::AddTimer(const void * arg, unsigned long stime, unsigned long interval)
{
	return m_Timer.AddTimer( static_cast<CGameEvent*>(const_cast<void*>(arg)) , stime , interval );
}

long   CTimer::run()
{
	
	while ( 1 )
	{
		m_Timer.Expired(  timeGetTime() );

		Sleep( 1 );
	}

	return 0;
}

