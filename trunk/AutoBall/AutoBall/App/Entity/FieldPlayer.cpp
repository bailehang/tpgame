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
#include "../StateAi/StateMachine.h"
#include "../StateAi/State.h"
#include "../../Render/Transformations.h"
#include "../../Render/Vector2D.h"
#include "../../Render/VGdi.h"
#include "../../Render/Geometry.h"
#include "../../Render/Utils.h"


extern luabind::object	g_states;

FieldPlayer::~FieldPlayer()
{
	SAFE_DELETE(m_pKickLimiter);
	SAFE_DELETE(m_pStateMachine);
	SAFE_DELETE(m_pStateMachineScript);
	
}

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
	m_pStateMachineScript = NULL;
	if (start_state)
	{    
		m_pStateMachine->SetCurrentState(start_state);
		m_pStateMachine->SetPreviousState(start_state);
		m_pStateMachine->SetGlobalState(&GetInstObj(GlobalPlayerState));

		m_pStateMachine->CurrentState()->Enter(this);
	}    

	m_pSteering->SeparationOn();

	//set up the kick regulator
	m_pKickLimiter = new Regulator(GetInstObj(CGameSetup).PlayerKickFrequency);
}

FieldPlayer::FieldPlayer(SoccerTeam*    home_team,
				int			   home_region,
				string		   State,
				Vector2D	   heading,
				Vector2D       velocity,
				double         mass,
				double         max_force,
				double         max_speed,
				double         max_turn_rate,
				double         scale,
				player_role    role): PlayerBase(home_team,
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
	 m_pStateMachineScript = new StateMachineScript<FieldPlayer>(this);
	 m_pStateMachine = NULL;
	 if( State != "" )
	 {
		m_pStateMachineScript->SetCurrentState( g_states[State] );
		m_pStateMachineScript->SetPreviousState(g_states[State] );
		m_pStateMachineScript->SetGlobalState ( g_states["State_GlobalPlayer"]);

		m_pStateMachineScript->EnterCurrentState();
	 }

	 m_pSteering->SeparationOn();

	//set up the kick regulator
	m_pKickLimiter = new Regulator(GetInstObj(CGameSetup).PlayerKickFrequency);
}

void FieldPlayer::Update()
{ 
	if( Team()->Color() == SoccerTeam::blue )
		m_pStateMachineScript->Update();
	else
	    m_pStateMachine->Update();

	m_pSteering->Calculate();

	/// 如果没有操作力，提供一个操作力
	if (m_pSteering->Force().IsZero())
	{
		const double BrakingRate = 0.8; 

		m_vVelocity = m_vVelocity * BrakingRate;                                     
	}

	/// 计算最大旋转力操作力
	double TurningForce =   m_pSteering->SideComponent();

	Clamp(TurningForce, -GetInstObj(CGameSetup).PlayerMaxTurnRate, GetInstObj(CGameSetup).PlayerMaxTurnRate);

	/// 旋转
	Vec2DRotateAroundOrigin(m_vHeading, TurningForce);

	/// 更新当前速度
	m_vVelocity = m_vHeading * m_vVelocity.Length();

	m_vSide = m_vHeading.Perp();


	Vector2D accel = m_vHeading * m_pSteering->ForwardComponent() / m_dMass;

	m_vVelocity += accel;

	/// 整理最大速度
	m_vVelocity.Truncate(m_dMaxSpeed);
	m_vPosition += m_vVelocity;

	//enforce a non-penetration constraint if desired
	if(GetInstObj(CGameSetup).bNonPenetrationConstraint)
	{
		EnforceNonPenetrationContraint(this, CAutoList<PlayerBase>::GetAllMembers());
	}
}


bool FieldPlayer::HandleMessage(const tagMessage& msg)
{	
	if( Team()->Color() == SoccerTeam::blue )
		return	m_pStateMachineScript->HandleMessage(msg);
	else
		return m_pStateMachine->HandleMessage(msg);
}

void FieldPlayer::Render()                                         
{
	GetInstObj(CGDI).TransparentText();
	GetInstObj(CGDI).TextColor(CGDI::grey);

	//set appropriate team color
	if (Team()->Color() == SoccerTeam::blue)
	{
		GetInstObj(CGDI).BluePen();
	}
	else
	{
		GetInstObj(CGDI).RedPen();
	}

	//render the player's body
	m_vecPlayerVBTrans = WorldTransform(m_vecPlayerVB,
										Pos(),
										Heading(),
										Side(),
										Scale());  
	GetInstObj(CGDI).ClosedShape(m_vecPlayerVBTrans); /**/ 

	//and 'is 'ead
	GetInstObj(CGDI).BrownBrush();
	if (GetInstObj(CGameSetup).bHighlightIfThreatened && 
		(Team()->ControllingPlayer() == this) && 
		isThreatened() ) 
		GetInstObj(CGDI).YellowBrush();

	GetInstObj(CGDI).Circle(Pos(), 5);

	/**
		GetInstObj(CGDI).TextColor(255, 0, 0);
		GetInstObj(CGDI).TextAtPos(Pos().x-3, Pos().y-5, ttos(GetID()) );
	 */

	//render the state
	if (GetInstObj(CGameSetup).bStates)
	{  
		GetInstObj(CGDI).TextColor(0, 170, 0);
		if( Team()->Color() != SoccerTeam::blue )
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
