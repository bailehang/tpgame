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
#include "Messageing/MessageDispatcher.h"
#include "../Public/Singleton.h"
#include "../Render/VGdi.h"
#include "../Render/Vector2D.h"
#include "../Render/Geometry.h"

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
	m_pStateMachine = new StateMachine<SoccerTeam>(this);

	m_pStateMachine->SetCurrentState(&GetInstObj(Defending));
	m_pStateMachine->SetPreviousState(&GetInstObj(Defending));
	m_pStateMachine->SetGlobalState(NULL);

	//create the players and goalkeeper
	CreatePlayers();

	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Steering()->SeparationOn();   
	}

	m_pSupportSpotCalc = new SupportSpotCalculator(GetInstObj(CGameSetup).NumSupportSpotsX,
							GetInstObj(CGameSetup).NumSupportSpotsY,
							this);

	m_IsChase = true;
}

SoccerTeam::~SoccerTeam()
{
	delete m_pStateMachine;

	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	for (it; it != m_Players.end(); ++it)
	{
		SAFE_DELETE( *it );
	}

	SAFE_DELETE( m_pSupportSpotCalc );
}

void SoccerTeam::Update()
{
	CalculateClosestPlayerToBall();

	m_pStateMachine->Update();

	//now update each player
	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Update();
	}

}

void SoccerTeam::CalculateClosestPlayerToBall()
{
	double ClosestSoFar = MaxFloat;

	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		double dist = Vec2DDistanceSq((*it)->Pos(), Pitch()->Ball()->Pos());

		(*it)->SetDistSqToBall(dist);

		if (dist < ClosestSoFar && (*it)->GetID()!=1 && (*it)->GetID()!=8 )
		{
			ClosestSoFar = dist;

			m_pPlayerClosestToBall = *it;
		}
	}

	m_dDistSqToBallOfClosestPlayer = ClosestSoFar;
}

PlayerBase* SoccerTeam::DetermineBestSupportingAttacker()
{

	/// 对所有队员进行遍历一次，求出基本值就Ok....
	double ClosestSoFar = MaxFloat;

	PlayerBase* BestPlayer = NULL;

	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		if ( ((*it)->Role() == PlayerBase::attacker) && ((*it) != m_pControllingPlayer) )
		{
			double dist = Vec2DDistanceSq((*it)->Pos(), m_pSupportSpotCalc->GetBestSupportingSpot());

			if ((dist < ClosestSoFar) )
			{
				ClosestSoFar = dist;

				BestPlayer = (*it);
			}
		}
	}

	return BestPlayer;
}

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
	for (curPlyr; curPlyr != Members().end(); ++curPlyr)
	{   
		/// 确保潜在的接球队员不是这个队员自己，切他所在位置与传球队员的距离大于最小传球距离
		if ( (*curPlyr != passer) &&            
			(Vec2DDistanceSq(passer->Pos(), (*curPlyr)->Pos()) > 
			MinPassingDistance*MinPassingDistance))                  
		{           
			if (GetBestPassToReceiver(passer, *curPlyr, Target, power))
			{
				///	如果传球目标是到目标位置所找到的离对方球门线最近的记录它
				double Dist2Goal = fabs(Target.x - OpponentsGoal()->Center().x);

				if (Dist2Goal < ClosestToGoalSoFar)
				{
					ClosestToGoalSoFar = Dist2Goal;

					/// 记录这个队员
					receiver = *curPlyr;

					/// 和这个目标
					PassTarget = Target;
				}     
			}
		}
	}//next team member

	if (receiver) return true;

	else return false;
}


bool SoccerTeam::GetBestPassToReceiver(const PlayerBase* const passer,
									   const PlayerBase* const receiver,
									   Vector2D&               PassTarget,
									   double                   power)const
{  
	/// 首先，计算球到达这个接球队员要花多长时间
	double time = Pitch()->Ball()->TimeToCoverDistance(Pitch()->Ball()->Pos(),
		receiver->Pos(),
		power);

	/// 如果在给定的力的作用下无法失球到达接球队员那里，返回假
	if (time < 0) return false;

	/// 这段时间中，接球队员能覆盖的最大距离
	double InterceptRange = time * receiver->MaxSpeed();

	/// 缩小截球范围
	const double ScalingFactor = 0.3;
	InterceptRange *= ScalingFactor;

	/// 计算在球道接球队员圈的切线范围内的传球目标
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

bool SoccerTeam::isPassSafeFromOpponent(Vector2D    from,
										Vector2D    target,
										const PlayerBase* const receiver,
										const PlayerBase* const opp,
										double       PassingForce)const
{
	/// 把对手移动本地空间
	Vector2D ToTarget = target - from;
	Vector2D ToTargetNormalized = Vec2DNormalize(ToTarget);

	Vector2D LocalPosOpp = PointToLocalSpace(opp->Pos(),
		ToTargetNormalized,
		ToTargetNormalized.Perp(),
		from);

	/// 如果对手在踢球者后面，那么被认为可以传球
	/// 这是基于这样的假设，球将被踢出的速度远大于对手的最大速度
	if ( LocalPosOpp.x < 0 )
	{     
		return true;
	}

	/// 如果对手到目标的距离更远
	/// 那么我们需要考虑对手可以接球队员先到达该位置
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
	double TimeForBall = 
		Pitch()->Ball()->TimeToCoverDistance(Vector2D(0,0),
		Vector2D(LocalPosOpp.x, 0),
		PassingForce);

	/// 现在计算在这段时间中对手能跑多远
	double reach = opp->MaxSpeed() * TimeForBall +
		Pitch()->Ball()->BRadius()+
		opp->BRadius();

	/// 如果到对手Y位置的距离小于他的跑动范围加上球半径和对手半径，那么求可以被截掉
	if ( fabs(LocalPosOpp.y) < reach )
	{
		return false;
	}

	return true;
}

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

bool SoccerTeam::CanShoot(Vector2D  BallPos,
						  double     power, 
						  Vector2D& ShotTarget)const
{

	/// 这个方法要测试的随机选取的射门目标的数量
	int NumAttempts = GetInstObj(CGameSetup).NumAttemptsToFindValidStrike;

	while (NumAttempts--)
	{
		/// 沿着对方的球门选这个一个随机位置（确保考虑了球的半径）
		ShotTarget = OpponentsGoal()->Center();

		/// 射门位置的Y值应该在两个球门柱之间（要考虑球的直径）
		int MinYVal = (int)(OpponentsGoal()->LeftPost().y + Pitch()->Ball()->BRadius());
		int MaxYVal = (int)(OpponentsGoal()->RightPost().y - Pitch()->Ball()->BRadius());

		ShotTarget.y = (double)RandInt(MinYVal, MaxYVal);

		/// 确保踢球力足够使球越过球门线
		double time = Pitch()->Ball()->TimeToCoverDistance(BallPos,
			ShotTarget,
			power);

		/// 如果是这样，测试这次射门是否会被任何对手截掉
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

void SoccerTeam::CreatePlayers()
{
  if (Color() == blue)
  {
    //goalkeeper
    m_Players.push_back(new GoalKeeper(this,
                               1,
							   &GetInstObj(TendGoal),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale));
 
	m_Players.push_back(new FieldPlayer(this,
                               22,
                               &GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               PlayerBase::attacker));

    //create the players
    m_Players.push_back(new FieldPlayer(this,
                               19,
                               &GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               PlayerBase::attacker));



        m_Players.push_back(new FieldPlayer(this,
                               16,
                               &GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               PlayerBase::attacker));
 

        m_Players.push_back(new FieldPlayer(this,
                               11,
                               &GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                              PlayerBase::defender));

		m_Players.push_back(new FieldPlayer(this,
                               8,
                               &GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               PlayerBase::defender));

		m_Players.push_back(new FieldPlayer(this,
                               6,
                               &GetInstObj(Wait),
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
                               45,
                               &GetInstObj(TendGoal),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale));


    //create the players
    m_Players.push_back(new FieldPlayer(this,
                               25,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               PlayerBase::attacker));

	//create the players
    m_Players.push_back(new FieldPlayer(this,
                               28,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               PlayerBase::attacker));

    m_Players.push_back(new FieldPlayer(this,
                               31,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               PlayerBase::attacker));


    m_Players.push_back(new FieldPlayer(this,
                               32,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               PlayerBase::defender));

	 m_Players.push_back(new FieldPlayer(this,
                               35,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               PlayerBase::defender));

	  m_Players.push_back(new FieldPlayer(this,
                               41,
                               &GetInstObj(Wait),
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

void SoccerTeam::UpdateTargetsOfWaitingPlayers()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{  
		if ( (*it)->Role() != PlayerBase::goal_keeper )
		{
			//cast to a field player
			FieldPlayer* plyr = static_cast<FieldPlayer*>(*it);

			if ( plyr->GetFSM()->isInState(*&GetInstObj(Wait)) ||
				plyr->GetFSM()->isInState(GetInstObj(ReturnToHomeRegion)) )
			{
				plyr->Steering()->SetTarget(plyr->HomeRegion()->Center());
			}
		}
	}
}

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

void SoccerTeam::RequestPass(FieldPlayer* requester)const
{
	//maybe put a restriction here
	if (RandFloat() > 0.1) return;

	if (isPassSafeFromAllOpponents(ControllingPlayer()->Pos(),
		requester->Pos(),
		requester,
		GetInstObj(CGameSetup).MaxPassingForce))
	{

		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			requester->GetID(),
			ControllingPlayer()->GetID(),
			Msg_PassToMe,
			requester); 

	}
}

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