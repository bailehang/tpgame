#include "stdafx.h"
#include "Regulator.h"


Regulator::Regulator(double NumUpdatesPerSecondRqd)
{
	m_dwNextUpdateTime = (DWORD)(timeGetTime()+RandFloat()*1000);

	if (NumUpdatesPerSecondRqd > 0)
	{
		m_dUpdatePeriod = 1000.0 / NumUpdatesPerSecondRqd; 
	}

	else if (isEqual(0.0, NumUpdatesPerSecondRqd))
	{
		m_dUpdatePeriod = 0.0;
	}

	else if (NumUpdatesPerSecondRqd < 0)
	{
		m_dUpdatePeriod = -1;
	}
}

bool Regulator::isReady()
{
	if (isEqual(0.0, m_dUpdatePeriod)) return true;

	if (m_dUpdatePeriod < 0) return false;

	DWORD CurrentTime = timeGetTime();

	static const double UpdatePeriodVariator = 10.0;

	if (CurrentTime >= m_dwNextUpdateTime)
	{
		m_dwNextUpdateTime = (DWORD)(CurrentTime + m_dUpdatePeriod + RandInRange(-UpdatePeriodVariator, UpdatePeriodVariator));

		return true;
	}

	return false;
}