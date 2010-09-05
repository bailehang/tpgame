
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

/// 调整者
class PlayerBase;
class Regulator
{
private:

	//the time period between updates 
	/// 2次更新间隔
	double m_dUpdatePeriod;

	//the next time the regulator allows code flow
	/// 下一次更新时间
	DWORD m_dwNextUpdateTime;

public:

	Regulator(double NumUpdatesPerSecondRqd)  ;

	bool isReady();
};
