
#pragma  once 

#include <string>
#include "State.h"

class GoalKeeper;
class SoccerPitch;

/// 守门员状态
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
/// 守球门
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

/// 截球
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

/// 回到起始位置
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

/// 把球传回到赛场中
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
