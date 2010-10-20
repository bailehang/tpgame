#include "stdafx.h"
#include "TimeCount.h"
#include "../Render/Utils.h"

TimeCount::TimeCount(double dUpdates)
{
	m_dwNextUpdateTime = (DWORD)(timeGetTime()+RandFloat()*1000);

	if (dUpdates > 0)
	{
		m_dUpdateNum = 1000.0 / dUpdates; 
	}

	else if (isEqual(0.0, dUpdates))
	{
		m_dUpdateNum = 0.0;
	}

	else if (dUpdates < 0)
	{
		m_dUpdateNum = -1;
	}
}

bool TimeCount::isReadyOK()
{
	if (isEqual(0.0, m_dUpdateNum)) return true;

	if (m_dUpdateNum < 0) return false;

	DWORD CurrentTime = timeGetTime();

	static const double sUpdateNum = 10.0;

	if (CurrentTime >= m_dwNextUpdateTime)
	{
		m_dwNextUpdateTime = (DWORD)(CurrentTime + m_dUpdateNum + RandInRange(-sUpdateNum, sUpdateNum));

		return true;
	}

	return false;
}