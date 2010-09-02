
#pragma  once 

#include "../Render/Vector2D.h"
#include "../Render/Utils.h"
#include "../Render/VGdi.h"

#include "../Public/Singleton.h"
#include "../Public/GameSetup.h"
#include "../Render/Utils.h"
#include "Region.h"
#include "SoccerPitch.h"
#include "SoccerTeam.h"
#include <mmsystem.h>

/// ��Ϊ��
class PlayerBase;
class Regulator
{
private:

	//the time period between updates 
	double m_dUpdatePeriod;

	//the next time the regulator allows code flow
	DWORD m_dwNextUpdateTime;


public:


	Regulator(double NumUpdatesPerSecondRqd)
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


	//returns true if the current time exceeds m_dwNextUpdateTime
	bool isReady()
	{
		//if a regulator is instantiated with a zero freq then it goes into
		//stealth mode (doesn't regulate)
		if (isEqual(0.0, m_dUpdatePeriod)) return true;

		//if the regulator is instantiated with a negative freq then it will
		//never allow the code to flow
		if (m_dUpdatePeriod < 0) return false;

		DWORD CurrentTime = timeGetTime();

		//the number of milliseconds the update period can vary per required
		//update-step. This is here to make sure any multiple clients of this class
		//have their updates spread evenly
		static const double UpdatePeriodVariator = 10.0;

		if (CurrentTime >= m_dwNextUpdateTime)
		{
			m_dwNextUpdateTime = (DWORD)(CurrentTime + m_dUpdatePeriod + RandInRange(-UpdatePeriodVariator, UpdatePeriodVariator));

			return true;
		}

		return false;
	}
};
