#include "Stdafx.h"
#include "BasePlayer.h"
#include "../Region.h"
#include "../FootBallTeam.h"
#include "../FootBallPitch.h"
#include "../Goal.h"
#include "../Steering.h"
#include "../Messageing/MessageDispatcher.h"
#include "../../Render/TransFuns.h"
#include "../../Render/Vector2D.h"
#include "../../Render/VGdi.h"
#include "../../Render/MathGeo.h"
#include "../../Render/Utils.h"

BasePlayer::~BasePlayer()
{
	SAFE_DELETE( m_pSteering );

}

BasePlayer::BasePlayer(FootBallTeam* home_team,
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

void BasePlayer::TrackBall()
{
	RotateHeadingToFacePosition(Ball()->Pos());  
}

void BasePlayer::TrackTarget()
{
	SetHeading(Vec2DNormalize(Steering()->Target() - Pos()));
}

bool  SortByDistanceToOpponentsGoal(const BasePlayer*const p1,
									const BasePlayer*const p2)
{
	return (p1->DistToOppGoal() < p2->DistToOppGoal());
}

bool  SortByReversedDistanceToOpponentsGoal(const BasePlayer*const p1,
											const BasePlayer*const p2)
{
	return (p1->DistToOppGoal() > p2->DistToOppGoal());
}

bool BasePlayer::PositionInFrontOfPlayer(Vector2D position)const
{
	Vector2D ToSubject = position - Pos();

	if (ToSubject.Dot(Heading()) > 0) 

		return true;

	else

		return false;
}

bool BasePlayer::isThreatened()const
{
	std::vector<BasePlayer*>::const_iterator curOpp;  
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

void BasePlayer::FindSupport()const
{    
	/// 查找最近玩家，让其转换为接应状态
	if (Team()->SupportingPlayer() == NULL)
	{
		BasePlayer* BestSupportPly = Team()->CalcBestSupportingAttacker();

		Team()->SetSupportingPlayer(BestSupportPly);

		Dispatcher->DispatchMsg(GetID(),
			Team()->SupportingPlayer()->GetID(),
			Msg_SupportAttacker,
			NULL);
	}

	BasePlayer* BestSupportPly = Team()->CalcBestSupportingAttacker();

	
	/// 如果当前位置适合让带球队员给自己传球，请求发球
	if (BestSupportPly && (BestSupportPly != Team()->SupportingPlayer()))
	{
		if (Team()->SupportingPlayer())
		{
			Dispatcher->DispatchMsg(GetID(),
				Team()->SupportingPlayer()->GetID(),
				Msg_GoHome,
				NULL);
		}

		Team()->SetSupportingPlayer(BestSupportPly);

		Dispatcher->DispatchMsg(GetID(),
			Team()->SupportingPlayer()->GetID(),
			Msg_SupportAttacker,
			NULL);
	}
}

double BasePlayer::DistToOppGoal()const
{
	return fabs(Pos().x - Team()->OpponentsGoal()->Center().x);
}

double BasePlayer::DistToHomeGoal()const
{
	return fabs(Pos().x - Team()->HomeGoal()->Center().x);
}

bool BasePlayer::isControllingPlayer()const
{
	return Team()->ControllingPlayer()==this;
}

bool BasePlayer::BallWithinKeeperRange()const
{
	return (Vec2DDistanceSq(Pos(), Ball()->Pos()) < 100 );
}

bool BasePlayer::BallWithinReceivingRange()const
{
	return (Vec2DDistanceSq(Pos(), Ball()->Pos()) < 100);
}

bool BasePlayer::BallWithinKickingRange()const
{
	return (Vec2DDistanceSq(Ball()->Pos(), Pos()) < GetInstObj(CGameSetup).PlayerKickingDistanceSq);
}


bool BasePlayer::InHomeRegion()const
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

bool BasePlayer::AtTarget()const
{
	return (Vec2DDistanceSq(Pos(), Steering()->Target()) < GetInstObj(CGameSetup).PlayerInTargetRangeSq);
}

bool BasePlayer::FollowTarget() const
{
	long len = (long)GetInstObj(CGameSetup).FollowMinDistenRange;
	return (Vec2DDistanceSq(Pos(), Ball()->Pos() ) < len*len );
}

bool BasePlayer::FollowReturn() const
{
	long len = (long)GetInstObj(CGameSetup).FollowMaxDistenRange;
	return (Vec2DDistanceSq(Pos(), HomeRegion()->Center() ) > len*len );
}

bool BasePlayer::IsSelfRegin() const
{
	return Vec2DDistanceSq(Pos(), HomeRegion()->Center() ) < 10;
}

bool BasePlayer::isClosestTeamMemberToBall()const
{
	return Team()->PlayerClosestToBall() == this;
}

bool BasePlayer::isClosestPlayerOnPitchToBall()const
{
	return isClosestTeamMemberToBall() && 
		(DistSqToBall() < Team()->Opponents()->ClosestDistToBallSq());
}

bool BasePlayer::InHotRegion()const
{
	return fabs(Pos().y - Team()->OpponentsGoal()->Center().y ) <
		Pitch()->PlayingArea()->Length()/3.0;
}

bool BasePlayer::isAheadOfAttacker()const
{
	return fabs(Pos().x - Team()->OpponentsGoal()->Center().x) <
		fabs(Team()->ControllingPlayer()->Pos().x - Team()->OpponentsGoal()->Center().x);
}

FootBall* const BasePlayer::Ball()const
{
	return Team()->Pitch()->Ball();
}

Vector2D   BasePlayer::BallPos() const
{
	return Ball()->Pos();
}

FootBallPitch* const BasePlayer::Pitch()const
{
	return Team()->Pitch();
}

void   BasePlayer::SetSteeringTarget( Vector2D& vec )
{
	Steering()->SetTarget( vec );
}

const Region* const BasePlayer::HomeRegion()const
{
	return Pitch()->GetRegionFromIndex(m_iHomeRegion);
}

bool   BasePlayer::InsideHomeRegion(Vector2D pos, int r)const
{
	 const Region* HomeRgn = HomeRegion();
	 return HomeRgn->Inside(pos,(Region::region_modifier)r);
}