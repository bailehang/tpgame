#include "Stdafx.h"
#include "PlayerBase.h"
#include "FieldPlayer.h"
#include "GoalKeeper.h"
#include "EntityFun.h"
#include "../Region.h"
#include "../SoccerTeam.h"
#include "../SoccerPitch.h"
#include "../Goal.h"		
#include "../SteeringBehaviors.h"
#include "../Messageing/MessageDispatcher.h"
#include "../Messageing/SoccerMessages.h"
#include "../StateAi/StateMachine.h"
#include "../StateAi/State.h"
#include "../StateAi/GoalKeeperStates.h"
#include "../StateAi/FieldPlayerStates.h"
#include "../../Render/Transformations.h"
#include "../../Render/Vector2D.h"
#include "../../Render/VGdi.h"
#include "../../Render/Geometry.h"
#include "../../Render/Utils.h"

FieldPlayer::~FieldPlayer()
{
	delete m_pKickLimiter;
	delete m_pStateMachine;
}

//----------------------------- ctor -------------------------------------
//------------------------------------------------------------------------
FieldPlayer::FieldPlayer(SoccerTeam* home_team,
						 int   home_region,
						 State<FieldPlayer>* start_state,
						 Vector2D  heading,
						 Vector2D velocity,
						 double    mass,
						 double    max_force,
						 double    max_speed,
						 double    max_turn_rate,
						 double    scale,
						 player_role role): PlayerBase(home_team,
						 home_region,
						 heading,
						 velocity,
						 mass,
						 max_force,
						 max_speed,
						 max_turn_rate,
						 scale,
						 role)                                    
{
	//set up the state machine
	m_pStateMachine =  new StateMachine<FieldPlayer>(this);

	if (start_state)
	{    
		m_pStateMachine->SetCurrentState(start_state);
		m_pStateMachine->SetPreviousState(start_state);
		m_pStateMachine->SetGlobalState(GlobalPlayerState::Instance());

		m_pStateMachine->CurrentState()->Enter(this);
	}    

	m_pSteering->SeparationOn();

	//set up the kick regulator
	m_pKickLimiter = new Regulator(GetInstObj(CGameSetup).PlayerKickFrequency);
}

//------------------------------ Update ----------------------------------
//
//  
//------------------------------------------------------------------------
void FieldPlayer::Update()
{ 
	//run the logic for the current state
	m_pStateMachine->Update();

	//calculate the combined steering force
	m_pSteering->Calculate();

	//if no steering force is produced decelerate the player by applying a
	//braking force
	if (m_pSteering->Force().IsZero())
	{
		const double BrakingRate = 0.8; 

		m_vVelocity = m_vVelocity * BrakingRate;                                     
	}

	//the steering force's side component is a force that rotates the 
	//player about its axis. We must limit the rotation so that a player
	//can only turn by PlayerMaxTurnRate rads per update.
	double TurningForce =   m_pSteering->SideComponent();

	Clamp(TurningForce, -GetInstObj(CGameSetup).PlayerMaxTurnRate, GetInstObj(CGameSetup).PlayerMaxTurnRate);

	//rotate the heading vector
	Vec2DRotateAroundOrigin(m_vHeading, TurningForce);

	//make sure the velocity vector points in the same direction as
	//the heading vector
	m_vVelocity = m_vHeading * m_vVelocity.Length();

	//and recreate m_vSide
	m_vSide = m_vHeading.Perp();


	//now to calculate the acceleration due to the force exerted by
	//the forward component of the steering force in the direction
	//of the player's heading
	Vector2D accel = m_vHeading * m_pSteering->ForwardComponent() / m_dMass;

	m_vVelocity += accel;

	//make sure player does not exceed maximum velocity
	m_vVelocity.Truncate(m_dMaxSpeed);

	//update the position
	m_vPosition += m_vVelocity;


	//enforce a non-penetration constraint if desired
	if(GetInstObj(CGameSetup).bNonPenetrationConstraint)
	{
		EnforceNonPenetrationContraint(this, CAutoList<PlayerBase>::GetAllMembers());
	}
}

//-------------------- HandleMessage -------------------------------------
//
//  routes any messages appropriately
//------------------------------------------------------------------------
bool FieldPlayer::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

//--------------------------- Render -------------------------------------
//
//------------------------------------------------------------------------
void FieldPlayer::Render()                                         
{
	GetInstObj(CGDI).TransparentText();
	GetInstObj(CGDI).TextColor(CGDI::grey);

	//set appropriate team color
	if (Team()->Color() == SoccerTeam::blue){GetInstObj(CGDI).BluePen();}
	else{GetInstObj(CGDI).RedPen();}



	//render the player's body
	m_vecPlayerVBTrans = WorldTransform(m_vecPlayerVB,
		Pos(),
		Heading(),
		Side(),
		Scale());
	GetInstObj(CGDI).ClosedShape(m_vecPlayerVBTrans);  

	//and 'is 'ead
	GetInstObj(CGDI).BrownBrush();
	if (GetInstObj(CGameSetup).bHighlightIfThreatened && (Team()->ControllingPlayer() == this) && isThreatened()) GetInstObj(CGDI).YellowBrush();
	GetInstObj(CGDI).Circle(Pos(), 6);


	//render the state
	if (GetInstObj(CGameSetup).bStates)
	{  
		GetInstObj(CGDI).TextColor(0, 170, 0);
		GetInstObj(CGDI).TextAtPos(m_vPosition.x, m_vPosition.y -20, std::string(m_pStateMachine->GetNameOfCurrentState()));
	}

	//show IDs
	if (GetInstObj(CGameSetup).bIDs)
	{
		GetInstObj(CGDI).TextColor(0, 170, 0);
		GetInstObj(CGDI).TextAtPos(Pos().x-20, Pos().y-20, ttos(GetID()));
	}


	if (GetInstObj(CGameSetup).bViewTargets)
	{
		GetInstObj(CGDI).RedBrush();
		GetInstObj(CGDI).Circle(Steering()->Target(), 3);
		GetInstObj(CGDI).TextAtPos(Steering()->Target(), ttos(GetID()));
	}   
}
