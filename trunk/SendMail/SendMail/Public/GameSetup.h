
#pragma  once 


class  CGameSetup
{

public:
	CGameSetup();
	~CGameSetup();

	void Reset();

public:

	double GoalWidth;	     /// «Ú√≈øÌ∂»
	int    NumSupportSpotsX;
	int    NumSupportSpotsY;
	double BallSize;
	double BallMass;
	double Friction;
	double PlayerMass;
	double PlayerMaxForce; 
	double PlayerMaxTurnRate;
	double PlayerScale;

	double PlayerKickingDistance;
	double PlayerKickingDistanceSq;
	double KeeperInBallRangeSq;
	double PlayerInTargetRangeSq;

	double PlayerComfortZoneSq;

	double WithinRangeOfSupportSpotSq;
	double BallWithinReceivingRange;
	double GoalKeeperInterceptRangeSq;
	double BallWithinReceivingRangeSq;
	double FollowMinDistenRange;
	double FollowMaxDistenRange;

	bool  bStates;
	bool  bIDs;
	bool  bRegions;
	bool  bShowControllingTeam;
	bool  bHighlightIfThreatened;

	int FrameRate;

	double SeparationCoefficient;

	double ViewDistance;
};