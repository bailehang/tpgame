#include "Stdafx.h"
#include "Steering.h"
#include "Entity/BasePlayer.h"
#include "Entity/FootBall.h"
#include "FootBallTeam.h"
#include "../Render/TransFuns.h"
#include "../Render/utils.h"
#include "../Public/Common/autolist.h"
#include "../Public/GameSetup.h"



using std::string;
using std::vector;


SteeringBehaviors::SteeringBehaviors(BasePlayer*  agent,
									 FootBallPitch* world,
									 FootBall*  ball):

m_pPlayer(agent),
m_iFlags(0),
m_dMultSeparation(GetInstObj(CGameSetup).SeparationCoefficient),
m_bTagged(false),
m_dViewDistance(GetInstObj(CGameSetup).ViewDistance),
m_pBall(ball),
m_dInterposeDist(0.0),
m_Antenna(5,Vector2D())
{
}


bool SteeringBehaviors::AccumulateForce(Vector2D &sf, Vector2D ForceToAdd)
{
	double MagnitudeSoFar = sf.Length();

	double magnitudeRemaining = m_pPlayer->MaxForce() - MagnitudeSoFar;

	if (magnitudeRemaining <= 0.0) return false;

	double MagnitudeToAdd = ForceToAdd.Length();

	if (MagnitudeToAdd > magnitudeRemaining)
	{
		MagnitudeToAdd = magnitudeRemaining;
	}

	sf += (Vec2DNormalize(ForceToAdd) * MagnitudeToAdd); 

	return true;
}

Vector2D SteeringBehaviors::Calculate()
{                                                                         
	m_vSteeringForce.Zero();

	m_vSteeringForce = SumForces();

	m_vSteeringForce.Truncate(m_pPlayer->MaxForce());

	return m_vSteeringForce;
}

Vector2D SteeringBehaviors::SumForces()
{
	Vector2D force;

	FindNeighbours();

	if (On(separation))
	{
		force += Separation() * m_dMultSeparation;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}    

	if (On(seek))
	{
		force += Seek(m_vTarget);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(arrive))
	{
		force += Arrive(m_vTarget, fast);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(pursuit))
	{
		force += Pursuit(m_pBall);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(interpose))
	{
		force += Interpose(m_pBall, m_vTarget, m_dInterposeDist);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	return m_vSteeringForce;
}

double SteeringBehaviors::ForwardComponent()
{
	return m_pPlayer->Heading().Dot(m_vSteeringForce);
}

double SteeringBehaviors::SideComponent()
{
	return m_pPlayer->Side().Dot(m_vSteeringForce) * m_pPlayer->MaxTurnRate();
}

Vector2D SteeringBehaviors::Seek(Vector2D target)
{

	Vector2D DesiredVelocity = Vec2DNormalize(target - m_pPlayer->Pos())
		* m_pPlayer->MaxSpeed();

	return (DesiredVelocity - m_pPlayer->Velocity());
}

Vector2D SteeringBehaviors::Arrive(Vector2D    target,
								   Deceleration deceleration)
{
	Vector2D ToTarget = target - m_pPlayer->Pos();

	double dist = ToTarget.Length();

	if (dist > 0)
	{
		const double DecelerationTweaker = 0.3;

		double speed =  dist / ((double)deceleration * DecelerationTweaker);                    

		speed = min(speed, m_pPlayer->MaxSpeed());

		Vector2D DesiredVelocity =  ToTarget * speed / dist;

		return (DesiredVelocity - m_pPlayer->Velocity());
	}

	return Vector2D(0,0);
}

Vector2D SteeringBehaviors::Pursuit(const FootBall* ball)
{
	Vector2D ToBall = ball->Pos() - m_pPlayer->Pos();

	double LookAheadTime = 0.0;

	if (ball->Speed() != 0.0)
	{
		LookAheadTime = ToBall.Length() / ball->Speed();
	}
	m_vTarget = ball->FuturePosition(LookAheadTime);

	return Arrive(m_vTarget, fast);
}

void SteeringBehaviors::FindNeighbours()
{
	std::list<BasePlayer*>& AllPlayers = CAutoList<BasePlayer>::GetAllMembers();
	std::list<BasePlayer*>::iterator curPlyr;
	for (curPlyr = AllPlayers.begin(); curPlyr!=AllPlayers.end(); ++curPlyr)
	{
		(*curPlyr)->Steering()->UnTag();

		Vector2D to = (*curPlyr)->Pos() - m_pPlayer->Pos();

		if (to.LengthSq() < (m_dViewDistance * m_dViewDistance))
		{
			(*curPlyr)->Steering()->Tag();
		}
	}//next
}

Vector2D SteeringBehaviors::Separation()
{  
	Vector2D SteeringForce;

	std::list<BasePlayer*>& AllPlayers = CAutoList<BasePlayer>::GetAllMembers();
	std::list<BasePlayer*>::iterator curPlyr;
	for (curPlyr = AllPlayers.begin(); curPlyr!=AllPlayers.end(); ++curPlyr)
	{
		if((*curPlyr != m_pPlayer) && (*curPlyr)->Steering()->Tagged())
		{
			Vector2D ToAgent = m_pPlayer->Pos() - (*curPlyr)->Pos();

			SteeringForce += Vec2DNormalize(ToAgent)/ToAgent.Length();
		}
	}

	return SteeringForce;
}

Vector2D SteeringBehaviors::Interpose(const FootBall* ball,
									  Vector2D  target,
									  double     DistFromTarget)
{
	return Arrive(target + Vec2DNormalize(ball->Pos() - target) * 
		DistFromTarget, normal);
}

void SteeringBehaviors::RenderAids( )
{ 
	GetInstObj(CGDI).RedPen();

	GetInstObj(CGDI).Line(m_pPlayer->Pos(), m_pPlayer->Pos() + m_vSteeringForce * 20);

}


