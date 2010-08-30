//#include "stdafx.h"

#include "GameSetup.h"
#include <fstream>
#include <iostream>
using namespace std;

CGameSetup::CGameSetup()
{
	ifstream file("setup.ini");

	if ( file.is_open() || file.eof() )
	{
		return ;
	}

	char str[100];
	if ( !file.eof() )
	{
		file >> str >>  GoalWidth 
			 >> str >>  NumSupportSpotsX
			 >> str >>  NumSupportSpotsY
			 >> str >>  Spot_PassSafeScore
			 >> str >>  Spot_CanScoreFromPositionScore
			 >> str >>  Spot_DistFromControllingPlayerScore
			 >> str >>  Spot_ClosenessToSupportingPlayerScore
			 >> str >>  Spot_AheadOfAttackerScore
			 >> str >>  SupportSpotUpdateFreq
			 >> str >>  ChancePlayerAttemptsPotShot
			 >> str >>  ChanceOfUsingArriveTypeReceiveBehavior
			 >> str >>  BallSize
			 >> str >>  BallMass
			 >> str >>  Friction
			 >> str >>  KeeperInBallRange
			 >> str >>  KeeperInBallRangeSq
			 >> str >>  PlayerInTargetRange
			 >> str >>  PlayerInTargetRangeSq
			 >> str >>  PlayerMass;
	}
}

CGameSetup::~CGameSetup()
{
	memset(this,0,sizeof(CGameSetup) );
}

void CGameSetup::Reset()
{

}