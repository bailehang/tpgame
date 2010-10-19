#include "Stdafx.h"
#include "PlayerBase.h"
#include "../Region.h"
#include "../SoccerTeam.h"
#include "../SoccerPitch.h"
#include "../Goal.h"
#include "../SteeringBehaviors.h"
#include "../Messageing/MessageDispatcher.h"
#include "../../Render/TransFuns.h"
#include "../../Render/Vector2D.h"
#include "../../Render/VGdi.h"
#include "../../Render/MathGeo.h"
#include "../../Render/Utils.h"

PlayerBase::~PlayerBase()
{
	SAFE_DELETE( m_pSteering );

}

PlayerBase::PlayerBase(SoccerTeam* home_team,
					   int   home_region,
					   Vector2D  heading,
					   Vector2D velocity,
					   double    mass,
					   double    max_force,
					   double    max_speed,
					   double    max_turn_rate,
					   double    scale,
					   player_role role):    

CMoveEntity(home_team->Pitch()->GetRegionFromIndex(home_region)->Center(),
			 scale*10.0,
			 velocity,
			 max_speed,
			 heading,
			 mass,
			 Vector2D(scale,scale),
			 max_turn_rate,
			 max_force),
			 m_pTeam(home_team),
			 m_dDistSqToBall(MaxFloat),
			 m_iHomeRegion(home_region),
			 m_iDefaultRegion(home_region),
			 m_PlayerRole(role)
{

	const int NumPlayerVerts = 4;
	const Vector2D player[NumPlayerVerts] = {Vector2D(-3, 8),
											 Vector2D(3,10),
											 Vector2D(3,-10),
											 Vector2D(-3,-8)};

	/// 计算边界比例
	for (int vtx=0; vtx<NumPlayerVerts; ++vtx)
	{
		m_vecPlayerVB.push_back(player[vtx]);

		if (abs(player[vtx].x) > m_dSize)
		{
			m_dSize = abs(player[vtx].x);
		}

		if (abs(player[vtx].y) > m_dSize)
		{
			m_dSize = abs(player[vtx].y);
		}
	}

	
	/// 操作行为
	m_pSteering = new SteeringBehaviors(this,
		m_pTeam->Pitch(),
		Ball());  

	/// 设定目标,初始状态为出生地
	m_pSteering->SetTarget(home_team->Pitch()->GetRegionFromIndex(home_region)->Center());
}

void PlayerBase::TrackBall()
{
	RotateHeadingToFacePosition(Ball()->Pos());  
}

void PlayerBase::TrackTarget()
{
	SetHeading(Vec2DNormalize(Steering()->Target() - Pos()));
}

bool  SortByDistanceToOpponentsGoal(const PlayerBase*const p1,
									const PlayerBase*const p2)
{
	return (p1->DistToOppGoal() < p2->DistToOppGoal());
}

bool  SortByReversedDistanceToOpponentsGoal(const PlayerBase*const p1,
											const PlayerBase*const p2)
{
	return (p1->DistToOppGoal() > p2->DistToOppGoal());
}

bool PlayerBase::PositionInFrontOfPlayer(Vector2D position)const
{
	Vector2D ToSubject = position - Pos();

	if (ToSubject.Dot(Heading()) > 0) 

		return true;

	else

		return false;
}

bool PlayerBase::isThreatened()const
{
	std::vector<PlayerBase*>::const_iterator curOpp;  
	curOpp = Team()->Opponents()->Members().begin();

	for (curOpp; curOpp != Team()->Opponents()->Members().end(); ++curOpp)
	{
		if (PositionInFrontOfPlayer((*curOpp)->Pos()) &&
			(Vec2DDistanceSq(Pos(), (*curOpp)->Pos()) < GetInstObj(CGameSetup).PlayerComfortZoneSq))
		{        
			return true;
		}

	}// next opp

	return false;
}

void PlayerBase::FindSupport()const
{    
	/// 查找最近玩家，让其转换为接应状态
	if (Team()->SupportingPlayer() == NULL)
	{
		PlayerBase* BestSupportPly = Team()->DetermineBestSupportingAttacker();

		Team()->SetSupportingPlayer(BestSupportPly);

		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			GetID(),
			Team()->SupportingPlayer()->GetID(),
			Msg_SupportAttacker,
			NULL);
	}

	PlayerBase* BestSupportPly = Team()->DetermineBestSupportingAttacker();

	
	/// 如果当前位置适合让带球队员给自己传球，请求发球
	if (BestSupportPly && (BestSupportPly != Team()->SupportingPlayer()))
	{
		if (Team()->SupportingPlayer())
		{
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
				GetID(),
				Team()->SupportingPlayer()->GetID(),
				Msg_GoHome,
				NULL);
		}

		Team()->SetSupportingPlayer(BestSupportPly);

		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			GetID(),
			Team()->SupportingPlayer()->GetID(),
			Msg_SupportAttacker,
			NULL);
	}
}

double PlayerBase::DistToOppGoal()const
{
	return fabs(Pos().x - Team()->OpponentsGoal()->Center().x);
}

double PlayerBase::DistToHomeGoal()const
{
	return fabs(Pos().x - Team()->HomeGoal()->Center().x);
}

bool PlayerBase::isControllingPlayer()const
{
	return Team()->ControllingPlayer()==this;
}

bool PlayerBase::BallWithinKeeperRange()const
{
	return (Vec2DDistanceSq(Pos(), Ball()->Pos()) < GetInstObj(CGameSetup).KeeperInBallRangeSq);
}

bool PlayerBase::BallWithinReceivingRange()const
{
	return (Vec2DDistanceSq(Pos(), Ball()->Pos()) < GetInstObj(CGameSetup).BallWithinReceivingRangeSq);
}

bool PlayerBase::BallWithinKickingRange()const
{
	return (Vec2DDistanceSq(Ball()->Pos(), Pos()) < GetInstObj(CGameSetup).PlayerKickingDistanceSq);
}


bool PlayerBase::InHomeRegion()const
{
	if (m_PlayerRole == goal_keeper)
	{
		return Pitch()->GetRegionFromIndex(m_iHomeRegion)->Inside(Pos(), Region::normal);
	}
	else
	{
		return Pitch()->GetRegionFromIndex(m_iHomeRegion)->Inside(Pos(), Region::halfsize);
	}
}

bool PlayerBase::AtTarget()const
{
	return (Vec2DDistanceSq(Pos(), Steering()->Target()) < GetInstObj(CGameSetup).PlayerInTargetRangeSq);
}

bool PlayerBase::FollowTarget() const
{
	long len = (long)GetInstObj(CGameSetup).FollowMinDistenRange;
	return (Vec2DDistanceSq(Pos(), Ball()->Pos() ) < len*len );
}

bool PlayerBase::FollowReturn() const
{
	long len = (long)GetInstObj(CGameSetup).FollowMaxDistenRange;
	return (Vec2DDistanceSq(Pos(), HomeRegion()->Center() ) > len*len );
}

bool PlayerBase::IsSelfRegin() const
{
	return Vec2DDistanceSq(Pos(), HomeRegion()->Center() ) < 10;
}

bool PlayerBase::isClosestTeamMemberToBall()const
{
	return Team()->PlayerClosestToBall() == this;
}

bool PlayerBase::isClosestPlayerOnPitchToBall()const
{
	return isClosestTeamMemberToBall() && 
		(DistSqToBall() < Team()->Opponents()->ClosestDistToBallSq());
}

bool PlayerBase::InHotRegion()const
{
	return fabs(Pos().y - Team()->OpponentsGoal()->Center().y ) <
		Pitch()->PlayingArea()->Length()/3.0;
}

bool PlayerBase::isAheadOfAttacker()const
{
	return fabs(Pos().x - Team()->OpponentsGoal()->Center().x) <
		fabs(Team()->ControllingPlayer()->Pos().x - Team()->OpponentsGoal()->Center().x);
}

SoccerBall* const PlayerBase::Ball()const
{
	return Team()->Pitch()->Ball();
}

Vector2D   PlayerBase::BallPos() const
{
	return Ball()->Pos();
}

SoccerPitch* const PlayerBase::Pitch()const
{
	return Team()->Pitch();
}

void   PlayerBase::SetSteeringTarget( Vector2D& vec )
{
	Steering()->SetTarget( vec );
}

const Region* const PlayerBase::HomeRegion()const
{
	return Pitch()->GetRegionFromIndex(m_iHomeRegion);
}

bool   PlayerBase::InsideHomeRegion(Vector2D pos, Region::region_modifier r)const
{
	 const Region* HomeRgn = HomeRegion();
	 return HomeRgn->Inside(pos,r);
}