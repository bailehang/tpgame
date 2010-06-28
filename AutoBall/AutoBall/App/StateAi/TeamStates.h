#pragma  once 

#include <string>

#include "State.h"
#include "../Telegram.h"


class SoccerTeam;

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
