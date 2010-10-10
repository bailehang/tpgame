#pragma  once 

#include <string>

#include "State.h"
#include "../Telegram.h"



					
CREATESTATE(Attacking,Telegram,SoccerTeam);

CREATESTATE(Defending,Telegram,SoccerTeam);

CREATESTATE(PrepareForKickOff,Telegram,SoccerTeam);

CREATESTATE(Throw_In,Telegram,SoccerTeam);


/*
/// 进攻状态
//------------------------------------------------------------------------
class Attacking : public State<SoccerTeam>
{ 
private:

	Attacking(){}

public:

	//this is a singleton
	static Attacking* Instance();

	void Enter(SoccerTeam* team);

	void Execute(SoccerTeam* team);

	void Exit(SoccerTeam* team);

	bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};

/// 防御，队伍归队，回到初始位置
//------------------------------------------------------------------------
class Defending : public State<SoccerTeam>
{ 
private:

	Defending(){}

public:

	//this is a singleton
	static Defending* Instance();

	void Enter(SoccerTeam* team);

	void Execute(SoccerTeam* team);

	void Exit(SoccerTeam* team);

	bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};

/// 准备开球
//------------------------------------------------------------------------
class PrepareForKickOff : public State<SoccerTeam>
{ 
private:

	PrepareForKickOff(){}

public:

	//this is a singleton
	static PrepareForKickOff* Instance();

	void Enter(SoccerTeam* team);

	void Execute(SoccerTeam* team);

	void Exit(SoccerTeam* team);

	bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};

///边界球
class Throw_In : public State<SoccerTeam>
{
private:

	Throw_In(){}

public:

	//this is a singleton
	static Throw_In* Instance();

	void Enter(SoccerTeam* team);

	void Execute(SoccerTeam* team);

	void Exit(SoccerTeam* team);

	bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};*/