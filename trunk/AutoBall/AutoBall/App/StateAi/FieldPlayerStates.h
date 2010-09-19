
#pragma  once
//------------------------------------------------------------------------
//
//  Name: FieldPlayerStates.h
//
//  Desc: States for the field players of Simple Soccer. See my book
//        for detailed descriptions
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <string>

#include "State.h"

class FieldPlayer;
class SoccerPitch;

/// 全局队员状态
//------------------------------------------------------------------------
class GlobalPlayerState : public State<FieldPlayer>
{
private:

	GlobalPlayerState(){}

public:

	//this is a singleton
	static GlobalPlayerState* Instance();

	void Enter(FieldPlayer* player){}

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player){}

	bool OnMessage(FieldPlayer*, const Telegram&);
};

/// 追球
//------------------------------------------------------------------------
class ChaseBall : public State<FieldPlayer>
{
private:

	ChaseBall(){}

public:

	//this is a singleton
	static ChaseBall* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// 带球
//------------------------------------------------------------------------
class Dribble : public State<FieldPlayer>
{
private:

	Dribble(){}

public:

	//this is a singleton
	static Dribble* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player){}

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// 归位
//------------------------------------------------------------------------
class ReturnToHomeRegion: public State<FieldPlayer>
{
private:

	ReturnToHomeRegion(){}

public:

	//this is a singleton
	static ReturnToHomeRegion* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// 等待
class Wait: public State<FieldPlayer>
{
private:

	Wait(){}

public:

	//this is a singleton
	static Wait* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};


/// 跟踪球，防御
class FollowBall :public State<FieldPlayer>
{
private:

	FollowBall() { }

public:

	//this is a singleton
	static FollowBall* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player){}

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}

};

/// 踢球
class KickBall: public State<FieldPlayer>
{
private:

	KickBall(){}

public:

	//this is a singleton
	static KickBall* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player){}

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// 接球
//------------------------------------------------------------------------
class ReceiveBall: public State<FieldPlayer>
{
private:

	ReceiveBall(){}

public:

	//this is a singleton
	static ReceiveBall* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// 接应
//------------------------------------------------------------------------
class SupportAttacker: public State<FieldPlayer>
{
private:

	SupportAttacker(){}

public:

	//this is a singleton
	static SupportAttacker* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

