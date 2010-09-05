

#pragma  once

#include "../../Public/Common/Autolist.h"
#include "MoveEntity.h"

class SoccerTeam;
class SoccerPitch;
class SoccerBall;
class SteeringBehaviors;
class Region;

/// 球员基类类 
class  PlayerBase : public  CMoveEntity,
					public  CAutoList<PlayerBase>
{

public:
	enum player_role{goal_keeper, attacker, defender};
public:
	PlayerBase(SoccerTeam*    home_team,
				int            home_region,
				Vector2D       heading,
				Vector2D       velocity,
				double          mass,
				double          max_force,
				double          max_speed,
				double          max_turn_rate,
				double          scale,
				player_role    role);

	virtual ~PlayerBase();


	//returns true if there is an opponent within this player's 
	//comfort zone
	bool        isThreatened()const;

	//rotates the player to face the ball or the player's current target
	void        TrackBall();
	void        TrackTarget();

	//this messages the player that is closest to the supporting spot to
	//change state to support the attacking player
	void        FindSupport()const;

	//returns true if the ball can be grabbed by the goalkeeper
	bool        BallWithinKeeperRange()const;

	//returns true if the ball is within kicking range
	bool        BallWithinKickingRange()const;

	//returns true if a ball comes within range of a receiver
	bool        BallWithinReceivingRange()const;

	//returns true if the player is located within the boundaries 
	//of his home region
	bool        InHomeRegion()const;

	//returns true if this player is ahead of the attacker
	bool        isAheadOfAttacker()const;

	//returns true if a player is located at the designated support spot
	bool        AtSupportSpot()const;

	//returns true if the player is located at his steering target
	bool        AtTarget()const;

	//returns true if the player is the closest player in his team to
	//the ball
	bool        isClosestTeamMemberToBall()const;

	//returns true if the point specified by 'position' is located in
	//front of the player
	bool        PositionInFrontOfPlayer(Vector2D position)const;

	//returns true if the player is the closest player on the pitch to the ball
	bool        isClosestPlayerOnPitchToBall()const;

	//returns true if this player is the controlling player
	bool        isControllingPlayer()const;

	//returns true if the player is located in the designated 'hot region' --
	//the area close to the opponent's goal
	bool        InHotRegion()const;

	player_role Role()const{return m_PlayerRole;}

	double      DistSqToBall()const{return m_dDistSqToBall;}
	void        SetDistSqToBall(double val){m_dDistSqToBall = val;}

	//calculate distance to opponent's/home goal. Used frequently by the passing
	//methods
	double       DistToOppGoal()const;
	double       DistToHomeGoal()const;

	void        SetDefaultHomeRegion(){m_iHomeRegion = m_iDefaultRegion;}

	SoccerBall*  const        Ball()const;
	SoccerPitch* const       Pitch()const;
	SteeringBehaviors*const  Steering()const{return m_pSteering;}
	const Region* const      HomeRegion()const;
	void                     SetHomeRegion(int NewRegion){m_iHomeRegion = NewRegion;}
	SoccerTeam*const         Team()const{return m_pTeam;}

protected:
	player_role				m_PlayerRole;

	//a pointer to this player's team
	SoccerTeam*             m_pTeam;

	//the steering behaviors
	SteeringBehaviors*      m_pSteering;

	//the region that this player is assigned to.
	int                     m_iHomeRegion;

	//the region this player moves to before kickoff
	int                     m_iDefaultRegion;

	//the distance to the ball (in squared-space). This value is queried 
	//a lot so it's calculated once each time-step and stored here.
	double                   m_dDistSqToBall;


	//the vertex buffer
	std::vector<Vector2D>   m_vecPlayerVB;
	//the buffer for the transformed vertices
	std::vector<Vector2D>   m_vecPlayerVBTrans;

};