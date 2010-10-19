/** 
 *	@brief 关于所有的函数注册
 *
 */
#include "stdafx.h"
#include "StateMachineScript.h"
#include "../FootBallPitch.h"
#include "../Entity/FootBaller.h"
#include "../Entity/FootBall.h"
#include "../Messageing/MessageDispatcher.h"
#include "../Steering.h"


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
			.def("RevNum",&tagMessage::RevNum)
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
			class_<StateMachineScript<FootBaller> >("StateMachineScript")

			.def("ChangeState",&StateMachineScript<FootBaller>::ChangeState)
			.def("SetCurrentState",&StateMachineScript<FootBaller>::SetCurrentState)
			.def("isInState",&StateMachineScript<FootBaller>::isInState)
		];
	//.def("OnMessage",&StateMachineScript<FootBaller>::OnMessage)
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
			class_<BasePlayer,bases<CMoveEntity> >("BasePlayer")

			.def("isThreatened",&BasePlayer::isThreatened)
			.def("IsChaseBall",&BasePlayer::IsChaseBall)
			.def("BallWithinReceivingRange",&BasePlayer::BallWithinReceivingRange)
			.def("isControllingPlayer",&BasePlayer::isControllingPlayer)
			.def("SetDefaultHomeRegion",&BasePlayer::SetDefaultHomeRegion)
			.def("FindSupport",&BasePlayer::FindSupport)
			.def("BallWithinKickingRange",&BasePlayer::BallWithinKickingRange)
			.def("isClosestTeamMemberToBall",&BasePlayer::isClosestTeamMemberToBall)
			.def("AtTarget",&BasePlayer::AtTarget)
			.def("TrackBall",&BasePlayer::TrackBall)
			.def("IsSelfRegin",&BasePlayer::IsSelfRegin)
			.def("FollowReturn",&BasePlayer::FollowReturn)
			.def("FollowTarget",&BasePlayer::FollowTarget)
			.def("InsideHomeRegion",&BasePlayer::InsideHomeRegion)
			.def("GetHomeCenter",&BasePlayer::GetHomeCenter)
			.def("InHotRegion",&BasePlayer::InHotRegion)
			.def("isAheadOfAttacker",&BasePlayer::isAheadOfAttacker)
			.def("BallPos",&BasePlayer::BallPos)
			.def("SetSteeringTarget",&BasePlayer::SetSteeringTarget)

			.def("Steering",&BasePlayer::Steering)
			.def("Ball",&BasePlayer::Ball)
			.def("Team",&BasePlayer::Team)
			.def("Pitch",&BasePlayer::Pitch)

		];
}

void  ReisterSoccerBall(lua_State* pLua)
{
	module(pLua)
		[
			class_<FootBall,bases<CMoveEntity> >("FootBall")

			.def("Kick",&FootBall::Kick)
		];
}

void  ReisterFieldPlayer(lua_State* pLua)
{
	module(pLua)
		[
			class_<FootBaller,bases<BasePlayer> >("FootBaller")

			.def("isReadyForNextKick",&FootBaller::isReadyForNextKick)
			.def("GetFSM",&FootBaller::GetScriptFSM)
			.def("GetScriptValue",&FootBaller::GetScriptValue)
			.def("SetScriptValue",&FootBaller::SetScriptValue)
		];
}


void  ReisterSteering(lua_State* pLua)
{
	module(pLua)
		[
			class_<SteeringBehaviors>("SteeringBehaviors")

			.def("SetTarget",&SteeringBehaviors::SetTarget)
			.def("Target",&SteeringBehaviors::Target)
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
			class_<FootBallTeam>("FootBallTeam")

			.def("CanShoot",&FootBallTeam::CanShoot)
			.def("InControl",&FootBallTeam::InControl)
			.def("IsControl",&FootBallTeam::IsControl)
			.def("GetSupportSpot",&FootBallTeam::GetSupportSpot)
			.def("RequestPass",&FootBallTeam::RequestPass)
			.def("Receiver",&FootBallTeam::Receiver)
			.def("SetReceiver",&FootBallTeam::SetReceiver)
			.def("InControl",&FootBallTeam::InControl)
			.def("SetControllingPlayer",&FootBallTeam::SetControllingPlayer)
			.def("SetThrowIn",&FootBallTeam::SetThrowIn)
			.def("HomeGoalFacing",&FootBallTeam::HomeGoalFacing)
			.def("IsReceiver",&FootBallTeam::IsReceiver)
			.def("IsThrowIn",&FootBallTeam::IsThrowIn)
			.def("SetThrowIn",&FootBallTeam::SetThrowIn)
			.def("isOpponentWithinRadius",&FootBallTeam::isOpponentWithinRadius)
		];
}

void  ReisterPitch(lua_State* pLua)
{
	module(pLua)
		[
			class_<FootBallPitch>("FootBallPitch")

			.def("GoalKeeperHasBall",&FootBallPitch::GoalKeeperHasBall)
			.def("GameOn",&FootBallPitch::GameOn)
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
			def("GetField",&GetExtraInfoField),
			def("VecAddr",&VecAddr),
			def("PrintLuaMsg",&PrintLuaMsg),
			def("GetGlobalEntityID",&GetGlobalEntityID)

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