#include "stdafx.h"
#include "SoccerTeam.h"
#include "SoccerPitch.h"
#include "Goal.h"
#include "Regulator.h"
#include "SupportSpotCalculator.h"
#include "SteeringBehaviors.h"
#include "Entity/PlayerBase.h"
#include "Entity/SoccerBall.h"
#include "Entity/GoalKeeper.h"
#include "Entity/FieldPlayer.h"
#include "Entity/EntityManager.h"
#include "StateAi/StateMachine.h"
#include "StateAi/TeamStates.h"
#include "StateAi/GoalKeeperStates.h"
#include "StateAi/FieldPlayerStates.h"
#include "Messageing/MessageDispatcher.h"
#include "Messageing/SoccerMessages.h"
#include "../Public/Singleton.h"
#include "../Render/VGdi.h"
#include "../Render/Vector2D.h"
#include "../Render/Geometry.h"

//----------------------------- ctor -------------------------------------
//
//------------------------------------------------------------------------
SoccerTeam::SoccerTeam(Goal*        home_goal,
					   Goal*        opponents_goal,
					   SoccerPitch* pitch,
					   team_color   color):m_pOpponentsGoal(opponents_goal),
					   m_pHomeGoal(home_goal),
					   m_pOpponents(NULL),
					   m_pPitch(pitch),
					   m_Color(color),
					   m_dDistSqToBallOfClosestPlayer(0.0),
					   m_pSupportingPlayer(NULL),
					   m_pReceivingPlayer(NULL),
					   m_pControllingPlayer(NULL),
					   m_pPlayerClosestToBall(NULL)
{
	//setup the state machine
	m_pStateMachine = new StateMachine<SoccerTeam>(this);

	m_pStateMachine->SetCurrentState(Defending::Instance());
	m_pStateMachine->SetPreviousState(Defending::Instance());
	m_pStateMachine->SetGlobalState(NULL);

	//create the players and goalkeeper
	CreatePlayers();

	//set default steering behaviors
	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Steering()->SeparationOn();   
	}

	//create the sweet spot calculator
	m_pSupportSpotCalc = new SupportSpotCalculator(GetInstObj(CGameSetup).NumSupportSpotsX,
		GetInstObj(CGameSetup).NumSupportSpotsY,
		this);

	m_IsChase = true;
}

//----------------------- dtor -------------------------------------------
//
//------------------------------------------------------------------------
SoccerTeam::~SoccerTeam()
{
	delete m_pStateMachine;

	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	for (it; it != m_Players.end(); ++it)
	{
		delete *it;
	}

	delete m_pSupportSpotCalc;
}

//-------------------------- update --------------------------------------
//
//  iterates through each player's update function and calculates 
//  frequently accessed info
//------------------------------------------------------------------------
void SoccerTeam::Update()
{
	//this information is used frequently so it's more efficient to 
	//calculate it just once each frame
	CalculateClosestPlayerToBall();

	//the team state machine switches between attack/defense behavior. It
	//also handles the 'kick off' state where a team must return to their
	//kick off positions before the whistle is blown
	m_pStateMachine->Update();

	//now update each player
	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Update();
	}

}


//------------------------ CalculateClosestPlayerToBall ------------------
//
//  sets m_iClosestPlayerToBall to the player closest to the ball
//------------------------------------------------------------------------
void SoccerTeam::CalculateClosestPlayerToBall()
{
	double ClosestSoFar = MaxFloat;

	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		//calculate the dist. Use the squared value to avoid sqrt
		double dist = Vec2DDistanceSq((*it)->Pos(), Pitch()->Ball()->Pos());

		if( dist <110 )
		{
			 (*it)->SetDistSqToBall(dist);
		}
		//keep a record of this value for each player
		(*it)->SetDistSqToBall(dist);

		if (dist < ClosestSoFar)
		{
			ClosestSoFar = dist;

			m_pPlayerClosestToBall = *it;
		}
	}

	m_dDistSqToBallOfClosestPlayer = ClosestSoFar;
}


//------------- DetermineBestSupportingAttacker ------------------------
//
// calculate the closest player to the SupportSpot
//------------------------------------------------------------------------
PlayerBase* SoccerTeam::DetermineBestSupportingAttacker()
{
	double ClosestSoFar = MaxFloat;

	PlayerBase* BestPlayer = NULL;

	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		//only attackers utilize the BestSupportingSpot
		if ( ((*it)->Role() == PlayerBase::attacker) && ((*it) != m_pControllingPlayer) )
		{
			//calculate the dist. Use the squared value to avoid sqrt
			double dist = Vec2DDistanceSq((*it)->Pos(), m_pSupportSpotCalc->GetBestSupportingSpot());

			//if the distance is the closest so far and the player is not a
			//goalkeeper and the player is not the one currently controlling
			//the ball, keep a record of this player
			if ((dist < ClosestSoFar) )
			{
				ClosestSoFar = dist;

				BestPlayer = (*it);
			}
		}
	}

	return BestPlayer;
}

//-------------------------- FindPass ------------------------------
//
//  The best pass is considered to be the pass that cannot be intercepted 
//  by an opponent and that is as far forward of the receiver as possible
//------------------------------------------------------------------------
bool SoccerTeam::FindPass(const PlayerBase*const passer,
						  PlayerBase*&           receiver,
						  Vector2D&              PassTarget,
						  double                  power,
						  double                  MinPassingDistance)const
{  


	std::vector<PlayerBase*>::const_iterator curPlyr = Members().begin();

	double    ClosestToGoalSoFar = MaxFloat;
	Vector2D Target;


	/// 迭代这个队员所在球队的所有队员，计算谁是接球对象
	//iterate through all this player's team members and calculate which
	//one is in a position to be passed the ball 
	for (curPlyr; curPlyr != Members().end(); ++curPlyr)
	{   
		/// 确保潜在的接球队员不是这个队员自己，切他所在位置与传球队员的距离大于最小传球距离
		//make sure the potential receiver being examined is not this player
		//and that it is further away than the minimum pass distance
		if ( (*curPlyr != passer) &&            
			(Vec2DDistanceSq(passer->Pos(), (*curPlyr)->Pos()) > 
			MinPassingDistance*MinPassingDistance))                  
		{           
			if (GetBestPassToReceiver(passer, *curPlyr, Target, power))
			{
				///	如果传球目标是到目标位置所找到的离对方球门线最近的记录它
				//if the pass target is the closest to the opponent's goal line found
				// so far, keep a record of it
				double Dist2Goal = fabs(Target.x - OpponentsGoal()->Center().x);

				if (Dist2Goal < ClosestToGoalSoFar)
				{
					ClosestToGoalSoFar = Dist2Goal;

					/// 记录这个队员
					//keep a record of this player
					receiver = *curPlyr;

					/// 和这个目标
					//and the target
					PassTarget = Target;
				}     
			}
		}
	}//next team member

	if (receiver) return true;

	else return false;
}


//---------------------- GetBestPassToReceiver ---------------------------
//
//  Three potential passes are calculated. One directly toward the receiver's
//  current position and two that are the tangents from the ball position
//  to the circle of radius 'range' from the receiver.
//  These passes are then tested to see if they can be intercepted by an
//  opponent and to make sure they terminate within the playing area. If
//  all the passes are invalidated the function returns false. Otherwise
//  the function returns the pass that takes the ball closest to the 
//  opponent's goal area.
//------------------------------------------------------------------------
bool SoccerTeam::GetBestPassToReceiver(const PlayerBase* const passer,
									   const PlayerBase* const receiver,
									   Vector2D&               PassTarget,
									   double                   power)const
{  
	/// 首先，计算球到达这个接球队员要花多长时间
	//first, calculate how much time it will take for the ball to reach 
	//this receiver, if the receiver was to remain motionless 
	double time = Pitch()->Ball()->TimeToCoverDistance(Pitch()->Ball()->Pos(),
		receiver->Pos(),
		power);

	/// 如果在给定的力的作用下无法失球到达接球队员那里，返回假
	//return false if ball cannot reach the receiver after having been
	//kicked with the given power
	if (time < 0) return false;

	/// 这段时间中，接球队员能覆盖的最大距离
	//the maximum distance the receiver can cover in this time
	double InterceptRange = time * receiver->MaxSpeed();

	/// 缩小截球范围
	//Scale the intercept range
	const double ScalingFactor = 0.3;
	InterceptRange *= ScalingFactor;

	/// 计算在球道接球队员圈的切线范围内的传球目标
	//now calculate the pass targets which are positioned at the intercepts
	//of the tangents from the ball to the receiver's range circle.
	Vector2D ip1, ip2;

	GetTangentPoints(receiver->Pos(),
		InterceptRange,
		Pitch()->Ball()->Pos(),
		ip1,
		ip2);

	const int NumPassesToTry = 3;
	Vector2D Passes[NumPassesToTry] = {ip1, receiver->Pos(), ip2};


	/// 目前为止这个传球的最佳的，如果
	/// 1.目前为止找到的比最近的有效传球更前场的位置
	/// 2.在赛场上
	/// 3.球不会被对方截中

	// this pass is the best found so far if it is:
	//
	//  1. Further upfield than the closest valid pass for this receiver
	//     found so far
	//  2. Within the playing area
	//  3. Cannot be intercepted by any opponents

	double ClosestSoFar = MaxFloat;
	bool  bResult      = false;

	for (int pass=0; pass<NumPassesToTry; ++pass)
	{    
		double dist = fabs(Passes[pass].x - OpponentsGoal()->Center().x);

		if (( dist < ClosestSoFar) &&
			Pitch()->PlayingArea()->Inside(Passes[pass]) &&
			isPassSafeFromAllOpponents(Pitch()->Ball()->Pos(),
			Passes[pass],
			receiver,
			power))

		{
			ClosestSoFar = dist;
			PassTarget   = Passes[pass];
			bResult      = true;
		}
	}

	return bResult;
}

//----------------------- isPassSafeFromOpponent -------------------------
//
//  test if a pass from 'from' to 'to' can be intercepted by an opposing
//  player
//------------------------------------------------------------------------
bool SoccerTeam::isPassSafeFromOpponent(Vector2D    from,
										Vector2D    target,
										const PlayerBase* const receiver,
										const PlayerBase* const opp,
										double       PassingForce)const
{
	/// 把对手移动本地空间
	//move the opponent into local space.
	Vector2D ToTarget = target - from;
	Vector2D ToTargetNormalized = Vec2DNormalize(ToTarget);

	Vector2D LocalPosOpp = PointToLocalSpace(opp->Pos(),
		ToTargetNormalized,
		ToTargetNormalized.Perp(),
		from);

	/// 如果对手在踢球者后面，那么被认为可以传球
	/// 这是基于这样的假设，球将被踢出的速度远大于对手的最大速度
	//if opponent is behind the kicker then pass is considered okay(this is 
	//based on the assumption that the ball is going to be kicked with a 
	//velocity greater than the opponent's max velocity)
	if ( LocalPosOpp.x < 0 )
	{     
		return true;
	}

	/// 如果对手到目标的距离更远
	/// 那么我们需要考虑对手可以接球队员先到达该位置
	//if the opponent is further away than the target we need to consider if
	//the opponent can reach the position before the receiver.
	if (Vec2DDistanceSq(from, target) < Vec2DDistanceSq(opp->Pos(), from))
	{
		/// 此条件语句放在这里是因为有事调用这个函数是，没有对接球者的引用
		/// 例如，你可能像知道球是否可以赶在对手之前到达场上的某个位置
		if (receiver)
		{
			if ( Vec2DDistanceSq(target, opp->Pos())  > 
				Vec2DDistanceSq(target, receiver->Pos()) )
			{
				return true;
			}

			else
			{
				return false;
			}

		}

		else
		{
			return true;
		} 
	}

	/// 计算球多久到达与对手正交的位置
	//calculate how long it takes the ball to cover the distance to the 
	//position orthogonal to the opponents position
	double TimeForBall = 
		Pitch()->Ball()->TimeToCoverDistance(Vector2D(0,0),
		Vector2D(LocalPosOpp.x, 0),
		PassingForce);

	/// 现在计算在这段时间中对手能跑多远
	//now calculate how far the opponent can run in this time
	double reach = opp->MaxSpeed() * TimeForBall +
		Pitch()->Ball()->BRadius()+
		opp->BRadius();

	/// 如果到对手Y位置的距离小于他的跑动范围加上球半径和对手半径，那么求可以被截掉
	//if the distance to the opponent's y position is less than his running
	//range plus the radius of the ball and the opponents radius then the
	//ball can be intercepted
	if ( fabs(LocalPosOpp.y) < reach )
	{
		return false;
	}

	return true;
}

//---------------------- isPassSafeFromAllOpponents ----------------------
//
//  tests a pass from position 'from' to position 'target' against each member
//  of the opposing team. Returns true if the pass can be made without
//  getting intercepted
//------------------------------------------------------------------------
bool SoccerTeam::isPassSafeFromAllOpponents(Vector2D                from,
											Vector2D                target,
											const PlayerBase* const receiver,
											double     PassingForce)const
{
	std::vector<PlayerBase*>::const_iterator opp = Opponents()->Members().begin();

	for (opp; opp != Opponents()->Members().end(); ++opp)
	{
		if (!isPassSafeFromOpponent(from, target, receiver, *opp, PassingForce))
		{
			//debug_on

				return false;
		}
	}

	return true;
}

//------------------------ CanShoot --------------------------------------
//
//  Given a ball position, a kicking power and a reference to a vector2D
//  this function will sample random positions along the opponent's goal-
//  mouth and check to see if a goal can be scored if the ball was to be
//  kicked in that direction with the given power. If a possible shot is 
//  found, the function will immediately return true, with the target 
//  position stored in the vector ShotTarget.
//------------------------------------------------------------------------
bool SoccerTeam::CanShoot(Vector2D  BallPos,
						  double     power, 
						  Vector2D& ShotTarget)const
{

	/// 这个方法要测试的随机选取的射门目标的数量
	//the number of randomly created shot targets this method will test 
	int NumAttempts = GetInstObj(CGameSetup).NumAttemptsToFindValidStrike;

	while (NumAttempts--)
	{
		/// 沿着对方的球门选这个一个随机位置（确保考虑了球的半径）
		//choose a random position along the opponent's goal mouth. (making
		//sure the ball's radius is taken into account)
		ShotTarget = OpponentsGoal()->Center();

		/// 射门位置的Y值应该在两个球门柱之间（要考虑球的直径）
		//the y value of the shot position should lay somewhere between two
		//goalposts (taking into consideration the ball diameter)
		int MinYVal = (int)(OpponentsGoal()->LeftPost().y + Pitch()->Ball()->BRadius());
		int MaxYVal = (int)(OpponentsGoal()->RightPost().y - Pitch()->Ball()->BRadius());

		ShotTarget.y = (double)RandInt(MinYVal, MaxYVal);

		/// 确保踢球力足够使球越过球门线
		//make sure striking the ball with the given power is enough to drive
		//the ball over the goal line.
		double time = Pitch()->Ball()->TimeToCoverDistance(BallPos,
			ShotTarget,
			power);

		/// 如果是这样，测试这次射门是否会被任何对手截掉
		//if it is, this shot is then tested to see if any of the opponents
		//can intercept it.
		if (time >= 0)
		{
			if (isPassSafeFromAllOpponents(BallPos, ShotTarget, NULL, power))
			{
				return true;
			}
		}
	}

	return false;
}


//--------------------- ReturnAllFieldPlayersToHome ---------------------------
//
//  sends a message to all players to return to their home areas forthwith
//------------------------------------------------------------------------
void SoccerTeam::ReturnAllFieldPlayersToHome()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		if ((*it)->Role() != PlayerBase::goal_keeper)
		{
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
				1,
				(*it)->GetID(),
				Msg_GoHome,
				NULL);
		}
	}
}


//--------------------------- Render -------------------------------------
//
//  renders the players and any team related info
//------------------------------------------------------------------------
void SoccerTeam::Render()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Render();
	}

	//show the controlling team and player at the top of the display
	if (GetInstObj(CGameSetup).bShowControllingTeam)
	{
		GetInstObj(CGDI).TextColor(CGDI::white);

		if ( (Color() == blue) && InControl())
		{
			GetInstObj(CGDI).TextAtPos(20,3,"Blue in Control");
		}
		else if ( (Color() == red) && InControl())
		{
			GetInstObj(CGDI).TextAtPos(20,3,"Red in Control");
		}
		if (m_pControllingPlayer != NULL)
		{
			GetInstObj(CGDI).TextAtPos(Pitch()->cxClient()-150, 3, "Controlling Player: " + ttos(m_pControllingPlayer->GetID()));
		}
	}

	//render the sweet spots
	if (GetInstObj(CGameSetup).bSupportSpots && InControl())
	{
		m_pSupportSpotCalc->Render();
	}

	//#define SHOW_TEAM_STATE
#ifdef SHOW_TEAM_STATE
	if (Color() == red)
	{
		GetInstObj(CGDI).TextColor(CGDI::white);

		if (CurrentState() == Attacking::Instance())
		{
			GetInstObj(CGDI).TextAtPos(160, 20, "Attacking");
		}
		if (CurrentState() == Defending::Instance())
		{
			GetInstObj(CGDI).TextAtPos(160, 20, "Defending");
		}
		if (CurrentState() == PrepareForKickOff::Instance())
		{
			GetInstObj(CGDI).TextAtPos(160, 20, "Kickoff");
		}
	}
	else
	{
		if (CurrentState() == Attacking::Instance())
		{
			GetInstObj(CGDI).TextAtPos(160, Pitch()->cyClient()-40, "Attacking");
		}
		if (CurrentState() == Defending::Instance())
		{
			GetInstObj(CGDI).TextAtPos(160, Pitch()->cyClient()-40, "Defending");
		}
		if (CurrentState() == PrepareForKickOff::Instance())
		{
			GetInstObj(CGDI).TextAtPos(160, Pitch()->cyClient()-40, "Kickoff");
		}
	}
#endif

	//#define SHOW_SUPPORTING_PLAYERS_TARGET
#ifdef SHOW_SUPPORTING_PLAYERS_TARGET
	if (m_pSupportingPlayer)
	{
		GetInstObj(CGDI).BlueBrush();
		GetInstObj(CGDI).RedPen();
		GetInstObj(CGDI).Circle(m_pSupportingPlayer->Steering()->Target(), 4);

	}
#endif

}

//------------------------- CreatePlayers --------------------------------
//
//  creates the players
//------------------------------------------------------------------------
void SoccerTeam::CreatePlayers()
{
	if (Color() == blue)
	{
		//goalkeeper
		m_Players.push_back(new GoalKeeper(this,
			1,
			TendGoal::Instance(),
			Vector2D(0,1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale));

		//create the players
		m_Players.push_back(new FieldPlayer(this,
			6,
			Wait::Instance(),
			Vector2D(0,1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale,
			PlayerBase::attacker));



		m_Players.push_back(new FieldPlayer(this,
			8,
			Wait::Instance(),
			Vector2D(0,1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale,
			PlayerBase::attacker));





		m_Players.push_back(new FieldPlayer(this,
			3,
			Wait::Instance(),
			Vector2D(0,1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale,
			PlayerBase::defender));


		m_Players.push_back(new FieldPlayer(this,
			5,
			Wait::Instance(),
			Vector2D(0,1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale,
			PlayerBase::defender));

	}

	else
	{

		//goalkeeper
		m_Players.push_back(new GoalKeeper(this,
			16,
			TendGoal::Instance(),
			Vector2D(0,-1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale));


		//create the players
		m_Players.push_back(new FieldPlayer(this,
			9,
			Wait::Instance(),
			Vector2D(0,-1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale,
			PlayerBase::attacker));

		m_Players.push_back(new FieldPlayer(this,
			11,
			Wait::Instance(),
			Vector2D(0,-1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale,
			PlayerBase::attacker));



		m_Players.push_back(new FieldPlayer(this,
			12,
			Wait::Instance(),
			Vector2D(0,-1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale,
			PlayerBase::defender));


		m_Players.push_back(new FieldPlayer(this,
			14,
			Wait::Instance(),
			Vector2D(0,-1),
			Vector2D(0.0, 0.0),
			GetInstObj(CGameSetup).PlayerMass,
			GetInstObj(CGameSetup).PlayerMaxForce,
			GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
			GetInstObj(CGameSetup).PlayerMaxTurnRate,
			GetInstObj(CGameSetup).PlayerScale,
			PlayerBase::defender));

	}

	//register the players with the entity manager
	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
	     GetInstObj(EntityManager).RegEntity(*it);
	}
}


PlayerBase* SoccerTeam::GetPlayerFromID(int id)const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		if ((*it)->GetID() == id) return *it;
	}

	return NULL;
}


void SoccerTeam::SetPlayerHomeRegion(int plyr, int region)const
{
	assert ( (plyr>=0) && (plyr<(int)m_Players.size()) );

	m_Players[plyr]->SetHomeRegion(region);
}


//---------------------- UpdateTargetsOfWaitingPlayers ------------------------
//
//  
void SoccerTeam::UpdateTargetsOfWaitingPlayers()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{  
		if ( (*it)->Role() != PlayerBase::goal_keeper )
		{
			//cast to a field player
			FieldPlayer* plyr = static_cast<FieldPlayer*>(*it);

			if ( plyr->GetFSM()->isInState(*Wait::Instance()) ||
				plyr->GetFSM()->isInState(*ReturnToHomeRegion::Instance()) )
			{
				plyr->Steering()->SetTarget(plyr->HomeRegion()->Center());
			}
		}
	}
}


//--------------------------- AllPlayersAtHome --------------------------------
//
//  returns false if any of the team are not located within their home region
//-----------------------------------------------------------------------------
bool SoccerTeam::AllPlayersAtHome()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		if ((*it)->InHomeRegion() == false)
		{
			return false;
		}
	}

	return true;
}

//------------------------- RequestPass ---------------------------------------
//
//  this tests to see if a pass is possible between the requester and
//  the controlling player. If it is possible a message is sent to the
//  controlling player to pass the ball asap.
//-----------------------------------------------------------------------------
void SoccerTeam::RequestPass(FieldPlayer* requester)const
{
	//maybe put a restriction here
	if (RandFloat() > 0.1) return;

	if (isPassSafeFromAllOpponents(ControllingPlayer()->Pos(),
		requester->Pos(),
		requester,
		GetInstObj(CGameSetup).MaxPassingForce))
	{

		//tell the player to make the pass
		//let the receiver know a pass is coming 
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			requester->GetID(),
			ControllingPlayer()->GetID(),
			Msg_PassToMe,
			requester); 

	}
}


//----------------------------- isOpponentWithinRadius ------------------------
//
//  returns true if an opposing player is within the radius of the position
//  given as a parameter
//-----------------------------------------------------------------------------
bool SoccerTeam::isOpponentWithinRadius(Vector2D pos, double rad)
{
	std::vector<PlayerBase*>::const_iterator end = Opponents()->Members().end();
	std::vector<PlayerBase*>::const_iterator it;

	for (it=Opponents()->Members().begin(); it !=end; ++it)
	{
		if (Vec2DDistanceSq(pos, (*it)->Pos()) < rad*rad)
		{
			return true;
		}
	}

	return false;
}