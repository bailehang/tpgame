#include "stdafx.h"
#include "BasePlayer.h"
#include "GoalKeeper.h"
#include "EntityFun.h"
#include "../Region.h"
#include "../FootBallTeam.h"
#include "../FootBallPitch.h"
#include "../Goal.h"
#include "../Steering.h"			
#include "../Messageing/MessageDispatcher.h"
#include "../StateAi/StateMachine.h"
#include "../StateAi/State.h"
#include "../../Render/TransFuns.h"
#include "../../Render/Vector2D.h"
#include "../../Render/VGdi.h"
#include "../../Render/MathGeo.h"
#include "../../Render/Utils.h"

GoalKeeper::GoalKeeper(FootBallTeam*        home_team,
					   int                home_region,
					   State<GoalKeeper>* start_state,
					   Vector2D           heading,
					   Vector2D           velocity,
					   double              mass,
					   double              max_force,
					   double              max_speed,
					   double              max_turn_rate,
					   double              scale): BasePlayer(home_team,
					   home_region,
					   heading,
					   velocity,
					   mass,
					   max_force,
					   max_speed,
					   max_turn_rate,
					   scale,
					   BasePlayer::goal_keeper)


{   
	//set up the state machine
	m_pStateMachine = new StateMachine<GoalKeeper>(this);

	m_pStateMachine->SetCurrentState(start_state);
	m_pStateMachine->SetPreviousState(start_state);
	m_pStateMachine->SetGlobalState(&GetInstObj(GlobalKeeperState));

	m_pStateMachine->CurrentState()->Enter(this);        
}



//-------------------------- Update --------------------------------------

void GoalKeeper::Update()
{ 
	//run the logic for the current state
	m_pStateMachine->Update();

	//calculate the combined force from each steering behavior 
	Vector2D SteeringForce = m_pSteering->Calculate();



	//Acceleration = Force/Mass
	Vector2D Acceleration = SteeringForce / m_dMass;

	//update velocity
	m_vVelocity += Acceleration;

	//make sure player does not exceed maximum velocity
	m_vVelocity.Truncate(m_dMaxSpeed);

	//update the position
	m_vPosition += m_vVelocity;

	//update the heading if the player has a non zero velocity
	if ( !m_vVelocity.IsZero())
	{    
		m_vHeading = Vec2DNormalize(m_vVelocity);

		m_vSide = m_vHeading.Perp();
	}

	//look-at vector always points toward the ball
	if (!Pitch()->GoalKeeperHasBall())
	{
		m_vLookAt = Vec2DNormalize(Ball()->Pos() - Pos());
	}
}


bool GoalKeeper::BallWithinRangeForIntercept()const
{
	return (Vec2DDistanceSq(Team()->HomeGoal()->Center(), Ball()->Pos()) <=
		GetInstObj(CGameSetup).GoalKeeperInterceptRangeSq);
}

bool GoalKeeper::TooFarFromGoalMouth()const
{
	return (Vec2DDistanceSq(Pos(), GetRearInterposeTarget()) >
		GetInstObj(CGameSetup).GoalKeeperInterceptRangeSq);
}

Vector2D GoalKeeper::GetRearInterposeTarget()const
{
	double xPosTarget = Team()->HomeGoal()->Center().x;

	double yPosTarget = Pitch()->PlayingArea()->Center().y - 
		GetInstObj(CGameSetup).GoalWidth*0.5 + (Ball()->Pos().y*GetInstObj(CGameSetup).GoalWidth) /
		Pitch()->PlayingArea()->Height();

	return Vector2D(xPosTarget, yPosTarget); 
}

//-------------------- OnMessage -------------------------------------
//
//  routes any messages appropriately
//------------------------------------------------------------------------
bool GoalKeeper::OnMessage(const tagMessage& msg)
{
	return m_pStateMachine->OnMessage(msg);
}

//--------------------------- Render -------------------------------------
//
//------------------------------------------------------------------------
void GoalKeeper::Render()                                         
{
	if (Team()->Color() == FootBallTeam::blue) 
		GetInstObj(CGDI).BluePen();
	else 
		GetInstObj(CGDI).RedPen();

	m_vecPlayerVBTrans = WorldTransform(m_vecPlayerVB,
		Pos(),
		m_vLookAt,
		m_vLookAt.Perp(),
		Scale());

	GetInstObj(CGDI).ClosedShape(m_vecPlayerVBTrans);

	//draw the head
	GetInstObj(CGDI).BrownBrush();
	GetInstObj(CGDI).Circle(Pos(), 6);

	//draw the ID
	if (GetInstObj(CGameSetup).bIDs)
	{
		GetInstObj(CGDI).TextColor(0, 170, 0);;
		GetInstObj(CGDI).TextAtPos(Pos().x-20, Pos().y-20, ttos(GetID()));
	}

	//draw the state
	if (GetInstObj(CGameSetup).bStates)
	{ 
		GetInstObj(CGDI).TextColor(0, 170, 0); 
		GetInstObj(CGDI).TransparentText(); 
		GetInstObj(CGDI).TextAtPos(m_vPosition.x, m_vPosition.y -20, 
			std::string(m_pStateMachine->GetNameOfCurrentState()));
	}
}