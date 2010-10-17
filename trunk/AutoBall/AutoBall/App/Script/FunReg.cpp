/** 
 *	@brief 关于所有的函数注册
 *
 */
#include "stdafx.h"
#include "StateMachineScript.h"
#include "../Entity/FieldPlayer.h"
#include "../Entity/SoccerBall.h"
#include "../Messageing/MessageDispatcher.h"
#include "../SteeringBehaviors.h"


void   ReisterVection(lua_State* pLua)
{
	module(pLua)
		[
			class_<Vector2D>("Vector2D")
			
			.def(constructor<>())
			.def("Dot",&Vector2D::Dot)
			.def("Sign",&Vector2D::Sign)
		];
}

void  ReisterTagMsg(lua_State* pLua)
{
	module(pLua)
		[
			class_<tagMessage>("tagMessage")

			.def("GetMsg",&tagMessage::GetMsg)
			.def("GetVec",&tagMessage::GetVec)
		];
}

void  ReisterMessageDispatcher(lua_State* pLua)
{
	module(pLua)
		[
			class_<MessageDispatcher>("MessageDispatcher")

			.def("DispatchMsg",&MessageDispatcher::DispatchMsg)
		];
}

void   ReisterStateMachine(lua_State* pLua)
{
	module(pLua)
		[
			class_<StateMachineScript<FieldPlayer> >("StateMachineScript")

			.def("ChangeState",&StateMachineScript<FieldPlayer>::ChangeState)
			.def("SetCurrentState",&StateMachineScript<FieldPlayer>::SetCurrentState)
			.def("isInState",&StateMachineScript<FieldPlayer>::isInState)
		];
	//.def("HandleMessage",&StateMachineScript<FieldPlayer>::HandleMessage)
}

void  ReisterBaseEntity(lua_State* pLua)
{
	module(pLua)
		[
			class_<CBaseEntity>("CBaseEntity")
				
			.def("Pos",&CBaseEntity::Pos)
			.def("ID",&CBaseEntity::GetID)
		];
}

void  ReisterMoveEntity(lua_State* pLua)
{
	module(pLua)
		[
			class_<CMoveEntity,bases<CBaseEntity> >("CMoveEntity")

			.def("SetMaxSpeed",&CMoveEntity::SetMaxSpeed)
			.def("SetVelocity",&CMoveEntity::SetVelocity)
			.def("Heading",&CMoveEntity::Heading)
		];
}


void  ReisterPlayerBase(lua_State* pLua)
{
	module(pLua)
		[
			class_<PlayerBase,bases<CMoveEntity> >("PlayerBase")

			.def("isThreatened",&PlayerBase::isThreatened)
			.def("IsChaseBall",&PlayerBase::IsChaseBall)
			.def("BallWithinReceivingRange",&PlayerBase::BallWithinReceivingRange)
			.def("isControllingPlayer",&PlayerBase::isControllingPlayer)
			.def("SetDefaultHomeRegion",&PlayerBase::SetDefaultHomeRegion)
			.def("FindSupport",&PlayerBase::FindSupport)
			.def("isClosestTeamMemberToBall",&PlayerBase::isClosestTeamMemberToBall)
			.def("AtTarget",&PlayerBase::AtTarget)
			.def("TrackBall",&PlayerBase::TrackBall)
			.def("IsSelfRegin",&PlayerBase::IsSelfRegin)
			.def("FollowReturn",&PlayerBase::FollowReturn)
			.def("FollowTarget",&PlayerBase::FollowTarget)
			.def("InsideHomeRegion",&PlayerBase::InsideHomeRegion)
			.def("GetHomeCenter",&PlayerBase::GetHomeCenter)
			.def("InHotRegion",&PlayerBase::InHotRegion)

			.def("Steering",&PlayerBase::Steering)
			.def("Ball",&PlayerBase::Ball)
			.def("Team",&PlayerBase::Team)
			.def("Pitch",&PlayerBase::Pitch)

		];
}

void  ReisterSoccerBall(lua_State* pLua)
{
	module(pLua)
		[
			class_<SoccerBall,bases<CMoveEntity> >("SoccerBall")

			.def("Kick",&SoccerBall::Kick)
		];
}

void  ReisterFieldPlayer(lua_State* pLua)
{
	module(pLua)
		[
			class_<FieldPlayer,bases<PlayerBase> >("FieldPlayer")

			.def("isReadyForNextKick",&FieldPlayer::isReadyForNextKick)
			.def("GetFSM",&FieldPlayer::GetScriptFSM)
		];
}


void  ReisterSteering(lua_State* pLua)
{
	module(pLua)
		[
			class_<SteeringBehaviors>("SteeringBehaviors")

			.def("SetTarget",&SteeringBehaviors::SetTarget)
			.def("SeekOn",&SteeringBehaviors::SeekOn)
			.def("SeekOff",&SteeringBehaviors::SeekOff)
			.def("ArriveOn",&SteeringBehaviors::ArriveOn)
			.def("ArriveOff",&SteeringBehaviors::ArriveOff)
			.def("PursuitOn",&SteeringBehaviors::PursuitOn)	
			.def("PursuitOff",&SteeringBehaviors::PursuitOff)
		];
}

void  ReisterTeam(lua_State* pLua)
{
	module(pLua)
		[
			class_<SoccerTeam>("SoccerTeam")

			.def("CanShoot",&SoccerTeam::CanShoot)
			.def("InControl",&SoccerTeam::InControl)
			.def("IsControl",&SoccerTeam::IsControl)
			.def("GetSupportSpot",&SoccerTeam::GetSupportSpot)
			.def("RequestPass",&SoccerTeam::RequestPass)
			.def("Receiver",&SoccerTeam::Receiver)
			.def("SetReceiver",&SoccerTeam::SetReceiver)
			.def("InControl",&SoccerTeam::InControl)
			.def("SetControllingPlayer",&SoccerTeam::SetControllingPlayer)
			.def("SetThrowIn",&SoccerTeam::SetThrowIn)
			.def("HomeGoalFacing",&SoccerTeam::HomeGoalFacing)
			.def("IsReceiver",&SoccerTeam::IsReceiver)
			.def("IsThrowIn",&SoccerTeam::IsThrowIn)
			.def("SetThrowIn",&SoccerTeam::SetThrowIn)
			.def("isOpponentWithinRadius",&SoccerTeam::isOpponentWithinRadius)
		];
}

void  ReisterPitch(lua_State* pLua)
{
	module(pLua)
		[
			class_<SoccerPitch>("SoccerPitch")

			.def("GoalKeeperHasBall",&SoccerPitch::GoalKeeperHasBall)
			.def("GameOn",&SoccerPitch::GameOn)
		];
}

void   ReisterAllFun(lua_State* pLua)
{
	module(pLua)
		[
			def("AddNoiseToKick",&AddNoiseToKick),
			def("RandFloat",&RandFloat),
			def("Vec2DRotateAroundOrigin",&Vec2DRotateAroundOrigin),
			def("MsgDispatcher",&MsgDispatcher),
			def("Vec2DNormalize",&Vec2DNormalize),
			def("Vec2DSub",&Vec2DSub),
			def("GetField",&GetExtraInfoField)

		];


	/// 绑定其他函数

	ReisterVection(pLua);
	ReisterTagMsg(pLua);
	ReisterStateMachine(pLua);
	ReisterBaseEntity(pLua);
	ReisterMoveEntity(pLua);
	ReisterPlayerBase(pLua);
	ReisterSoccerBall(pLua);
	ReisterFieldPlayer(pLua);
	ReisterSteering(pLua);
	ReisterTeam(pLua);
	ReisterPitch(pLua);
}