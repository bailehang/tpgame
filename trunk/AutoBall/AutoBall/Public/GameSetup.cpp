#include "stdafx.h"

#include "GameSetup.h"
#include <fstream>
#include <iostream>
using namespace std;

CGameSetup::CGameSetup()
{
	ifstream file("setup.ini");

	if ( !file.is_open() || file.eof() )
	{
		return ;
	}

	char str[100];
	if ( !file.eof() )
	{

		file >> str >>  GoalWidth 
			 >> str >>  NumSupportSpotsX
			 >> str >>  NumSupportSpotsY
			 >> str >>  BallSize
			 >> str >>  BallMass
			 >> str >>  Friction
			 >> str >>  PlayerMass
			 >> str >>  PlayerMaxForce 
			 >> str >>  PlayerMaxTurnRate
			 >> str >>  PlayerScale
			 >> str >>  BallWithinReceivingRange
			 >> str >>  FollowMinDistenRange
			 >> str >>	FollowMaxDistenRange
			 >> str >>  bStates
			 >> str >>  bIDs
			 >> str >>  bRegions
			 >> str >>  bShowControllingTeam
			 >> str >>  bHighlightIfThreatened
			 >> str >>  FrameRate
			 >> str >>  SeparationCoefficient
			 >> str >>  ViewDistance;

	}
	BallWithinReceivingRangeSq = 100;
	KeeperInBallRangeSq      = 10*10;
	PlayerInTargetRangeSq    = 100;   
	PlayerKickingDistance    += BallSize;
	PlayerKickingDistanceSq  = 100;
	PlayerComfortZoneSq      = 60*60;
	GoalKeeperInterceptRangeSq = 100 * 100.0;
	WithinRangeOfSupportSpotSq = 15*15;
}

CGameSetup::~CGameSetup()
{
	memset(this,0,sizeof(CGameSetup) );
}

void CGameSetup::Reset()
{

}