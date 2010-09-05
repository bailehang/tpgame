#ifndef SUPPORTSPOTCALCULATOR
#define SUPPORTSPOTCALCULATOR
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   SupportSpotCalculator.h
//
//  Desc:   Class to determine the best spots for a suppoting soccer
//          player to move to.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <vector>

#include "Regulator.h"
#include "Region.h"
#include "../Render/Vector2D.h"
#include "../Render/VGdi.h"


class PlayerBase;
class Goal;
class SoccerBall;
class SoccerTeam;
class Regulator;

/// 行为计算类
class SupportSpotCalculator
{
private:

	/// 计算最佳接应点
	//a data structure to hold the values and positions of each spot
	struct SupportSpot
	{

		Vector2D  m_vPos;

		double    m_dScore;

		SupportSpot(Vector2D pos, double value):m_vPos(pos),
			m_dScore(value)
		{}
	};

private:


	SoccerTeam*               m_pTeam;	 /// 队

	std::vector<SupportSpot>  m_Spots;	 /// 所有的传送点

	//a pointer to the highest valued spot from the last update
	SupportSpot*              m_pBestSupportingSpot;	  /// 最佳传送点

	//this will regulate how often the spots are calculated (default is
	//one update per second)
	Regulator*                m_pRegulator;///球传送的过程中，每几帧更新 

public:

	SupportSpotCalculator(int numX,
		int numY,
		SoccerTeam* team);

	~SupportSpotCalculator();

	//draws the spots to the screen as a hollow circles. The higher the 
	//score, the bigger the circle. The best supporting spot is drawn in
	//bright green.
	void       Render()const;

	//this method iterates through each possible spot and calculates its
	//score.			
	/// 决定哪个点是最佳的传射点，接应队员到达那个位置，等待传球
	Vector2D  DetermineBestSupportingPosition();

	//returns the best supporting spot if there is one. If one hasn't been
	//calculated yet, this method calls DetermineBestSupportingPosition and
	//returns the result.
	/// 返回最佳传送点
	Vector2D  GetBestSupportingSpot();
};


#endif