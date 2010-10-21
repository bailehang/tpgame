#pragma once

// 实体类型
enum	eEntityType
{
	Ent_InValid	=	-1,
	Ent_Goal  ,			 /// 守门员
	Ent_FootBaller		 /// 球员
};

enum   ePlayerType
{
	default_entity_type,
	ePlayer_GoalKeeper,
	ePlayer_Attacker,
	ePlayer_Defender
};

// 状态类型
enum	eStateAi
{
	Ent_InValidState= -1,
	Ent_PeaceState	   ,
	Ent_FleeState	   ,
	Ent_FightState	   ,
	Ent_FollowState	   ,
	Ent_DeadState	   ,
	Ent_GoHomeState	   ,


	Ent_StateNumer			
};

enum  eEventType
{
	Msg_ReceiveBall,
	Msg_PassToMe,
	Msg_SupportAttacker,
	Msg_GoHome,
	Msg_Wait
};
