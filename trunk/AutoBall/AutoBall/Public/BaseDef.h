#pragma once

// 实体类型
enum	eEntityType
{
	Ent_InValid	=	-1,
	Ent_Goal  ,			 /// 守门员
	Ent_FieldPlayer		 /// 球员
};

enum   ePlayerType
{
	ePlayer_GoalKeeper,
	ePlayer_Attacker,
	ePlayer_Defender
};

// 状态类型
enum	eStateAi
{
	Ent_InValidState=-1,
	Ent_PeaceState	   ,
	Ent_FleeState	   ,
	Ent_FightState	   ,
	Ent_FollowState	   ,
	Ent_DeadState	   ,
	Ent_GoHomeState	   ,


	Ent_StateNumer			
};


// 事件类型
enum	eEventAi
{
	Ent_InValidEvent=-1,
	Ent_MoveEvent	= 0,
	Ent_StopEvent	   ,
	Ent_UseSkillEvent  ,


	Ent_EventNum
};


struct  tagEevent
{
	void*		Sender;
	void*		Receiver;

	eEventAi	eEvent;
};