
#pragma  once 


class  CGameSetup
{

public:
	CGameSetup();
	~CGameSetup();

	void Reset();

public:

	double GoalWidth;
	int   NumSupportSpotsX;
	int   NumSupportSpotsY;

	double Spot_PassSafeScore;
	double Spot_CanScoreFromPositionScore;
	double Spot_DistFromControllingPlayerScore;
	double Spot_ClosenessToSupportingPlayerScore;
	double Spot_AheadOfAttackerScore;  
	double SupportSpotUpdateFreq ;
	double ChancePlayerAttemptsPotShot; 
	double ChanceOfUsingArriveTypeReceiveBehavior;

	double BallSize;
	double BallMass;
	double Friction;
	double KeeperInBallRange;
	double KeeperInBallRangeSq;
	double PlayerInTargetRange;
	double PlayerInTargetRangeSq;
	double PlayerMass;

	double PlayerMaxForce; 
	double PlayerMaxSpeedWithBall;
	double PlayerMaxSpeedWithoutBall;
	double PlayerMaxTurnRate;
	double PlayerScale;
	double PlayerComfortZone;

	double PlayerKickingDistance;
	double PlayerKickingDistanceSq;
	double PlayerKickFrequency; 
	double MaxDribbleForce;
	double MaxShootingForce;
	double MaxPassingForce;
	double PlayerComfortZoneSq;
	double PlayerKickingAccuracy;

	int    NumAttemptsToFindValidStrike;

	double WithinRangeOfHome;
	double WithinRangeOfSupportSpot;
	double WithinRangeOfSupportSpotSq;

	double MinPassDist;
	double GoalkeeperMinPassDist;
	double GoalKeeperTendingDistance;
	double GoalKeeperInterceptRange;
	double GoalKeeperInterceptRangeSq;
	double BallWithinReceivingRange;
	double BallWithinReceivingRangeSq;
	double FollowMinDistenRange;
	double FollowMaxDistenRange;

	bool  bStates;
	bool  bIDs;
	bool  bSupportSpots;
	bool  bRegions;
	bool  bShowControllingTeam;
	bool  bViewTargets;
	bool  bHighlightIfThreatened;

	int FrameRate;


	double SeparationCoefficient;

	//how close a neighbour must be before an agent perceives it
	double ViewDistance;

	//zero this to turn the constraint off
	bool bNonPenetrationConstraint;
};