
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


class PlayerBase;

/// 关于是否准备进行下一次踢球
class Regulator
{

public:

	Regulator(double NumUpdatesPerSecondRqd)  ;

	bool isReady();

private:

	/// 2次更新间隔
	double m_dUpdatePeriod;

	/// 下一次更新时间
	DWORD m_dwNextUpdateTime;
};
