#ifndef SUPPORTSPOTCALCULATOR
#define SUPPORTSPOTCALCULATOR
#pragma warning (disable:4786)

#include <vector>

#include "TimeCount.h"
#include "Region.h"
#include "../Render/Vector2D.h"
#include "../Render/VGdi.h"

class BasePlayer;
class Goal;
class FootBall;
class FootBallTeam;
class TimeCount;

/// 行为计算类
class SupportSpotCalculator
{
private:

	/// 计算最佳接应点
	struct SupportSpot
	{

		Vector2D  m_vPos;

		double    m_dScore;

		SupportSpot(Vector2D pos, double value):m_vPos(pos),
			m_dScore(value)
		{}
	};

private:

	FootBallTeam*               m_pTeam;				///> 队
	std::vector<SupportSpot>  m_Spots;				///> 所有的传送点
	SupportSpot*              m_pBestSupportingSpot;///> 最佳传送点
	TimeCount*                m_pPassTimer;			///> 球传送的过程中，每几帧更新 

public:

	SupportSpotCalculator(int numX,
						  int numY,
						  FootBallTeam* team);

	~SupportSpotCalculator();

	void       Render()const;
			
	/// 决定哪个点是最佳的传射点，接应队员到达那个位置，等待传球
	Vector2D  DetermineBestSupportingPosition();

	/// 返回最佳传送点
	Vector2D  GetBestSupportingSpot();
};


#endif