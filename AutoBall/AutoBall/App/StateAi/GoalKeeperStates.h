
#pragma  once 

#include <string>
#include "State.h"
#include "../Common/Messaging/Telegram.h"
#include "constants.h"


class GoalKeeper;
class SoccerPitch;

class GlobalKeeperState: public State<GoalKeeper>
{
private:

	GlobalKeeperState(){}

public:

	//this is a singleton
	static GlobalKeeperState* Instance();

	void Enter(GoalKeeper* keeper){}

	void Execute(GoalKeeper* keeper){}

	void Exit(GoalKeeper* keeper){}

	bool OnMessage(GoalKeeper*, const Telegram&);
};

//-----------------------------------------------------------------------------

class TendGoal: public State<GoalKeeper>
{
private:

	TendGoal(){}

public:

	//this is a singleton
	static TendGoal* Instance();

	void Enter(GoalKeeper* keeper);

	void Execute(GoalKeeper* keeper);

	void Exit(GoalKeeper* keeper);

	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class InterceptBall: public State<GoalKeeper>
{
private:

	InterceptBall(){}

public:

	//this is a singleton
	static InterceptBall* Instance();

	void Enter(GoalKeeper* keeper);

	void Execute(GoalKeeper* keeper);

	void Exit(GoalKeeper* keeper);

	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class ReturnHome: public State<GoalKeeper>
{
private:

	ReturnHome(){}

public:

	//this is a singleton
	static ReturnHome* Instance();

	void Enter(GoalKeeper* keeper);

	void Execute(GoalKeeper* keeper);

	void Exit(GoalKeeper* keeper);

	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class PutBallBackInPlay: public State<GoalKeeper>
{
private:

	PutBallBackInPlay(){}

public:

	//this is a singleton
	static PutBallBackInPlay* Instance();

	void Enter(GoalKeeper* keeper);

	void Execute(GoalKeeper* keeper);

	void Exit(GoalKeeper* keeper){}

	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};
