#include "Stdafx.h"
#include "Timer.h"


CTimer::CTimer(long fps)
{
	m_lFps = fps;
}

void  CTimer::Start()
{
	m_bStarted   = true;
	m_lStartTime = 0;
}


bool  CTimer::ReadyForNextFrame() 
{
	long lNow = timeGetTime();
	if( m_bStarted && (lNow-m_lStartTime) > m_lFps )
	{
		m_lStartTime = lNow;
		return true;
	}

	return false;
}
