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
			 >> str >>  PlayerInTargetRange
			 >> str >>  PlayerKickingDistance
			 >> str >>  PlayerKickFrequency
			 >> str >>  PlayerMass
			 >> str >>  PlayerMaxForce 
			 >> str >>  PlayerMaxSpeedWithBall
			 >> str >>  PlayerMaxSpeedWithoutBall
			 >> str >>  PlayerMaxTurnRate
			 >> str >>  PlayerScale
			 >> str >>  PlayerComfortZone
			 >> str >>  PlayerKickingAccuracy
			 >> str >>  NumAttemptsToFindValidStrike
			 >> str >>  MaxDribbleForce
			 >> str >>  MaxShootingForce
			 >> str >>  MaxPassingForce
			 >> str >>  WithinRangeOfHome
			 >> str >>  WithinRangeOfSupportSpot
			 >> str >>  MinPassDist
			 >> str >>  GoalkeeperMinPassDist
			 >> str >>  GoalKeeperTendingDistance
			 >> str >>  GoalKeeperInterceptRange
			 >> str >>  BallWithinReceivingRange
			 >> str >>  FollowMinDistenRange
			 >> str >>	FollowMaxDistenRange
			 >> str >>  bStates
			 >> str >>  bIDs
			 >> str >>  bSupportSpots
			 >> str >>  bRegions
			 >> str >>  bShowControllingTeam
			 >> str >>  bViewTargets
			 >> str >>  bHighlightIfThreatened
			 >> str >>  FrameRate
			 >> str >>  SeparationCoefficient
			 >> str >>  ViewDistance
			 >> str >>  bNonPenetrationConstraint;

	}
	BallWithinReceivingRangeSq = BallWithinReceivingRange * BallWithinReceivingRange;
	KeeperInBallRangeSq      = KeeperInBallRange * KeeperInBallRange;
	PlayerInTargetRangeSq    = PlayerInTargetRange * PlayerInTargetRange;   
	PlayerKickingDistance   += BallSize;
	PlayerKickingDistanceSq  = PlayerKickingDistance * PlayerKickingDistance;
	PlayerComfortZoneSq      = PlayerComfortZone * PlayerComfortZone;
	GoalKeeperInterceptRangeSq     = GoalKeeperInterceptRange * GoalKeeperInterceptRange;
	WithinRangeOfSupportSpotSq = WithinRangeOfSupportSpot * WithinRangeOfSupportSpot;
}

CGameSetup::~CGameSetup()
{
	memset(this,0,sizeof(CGameSetup) );
}

void CGameSetup::Reset()
{

}