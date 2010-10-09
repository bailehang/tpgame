
#pragma  once

#include <string>

#include "State.h"

class FieldPlayer;
class SoccerPitch;


/// 全局队员状态
CREATESTATE(GlobalPlayerState,Telegram);

/// 追求状态
CREATESTATE(ChaseBall,Telegram);

/// 带球状态
CREATESTATE(Dribble,Telegram);

/// 回到出生位置
CREATESTATE(ReturnToHomeRegion,Telegram);

/// 等待状态
CREATESTATE(Wait,Telegram);

/// 跟踪球位置
CREATESTATE(FollowBall,Telegram);

/// 
CREATESTATE(KickBall,Telegram);

///
CREATESTATE(ReceiveBall,Telegram);

///
CREATESTATE(SupportAttacker,Telegram);

// 
// 
// /// 全局队员状态
// //------------------------------------------------------------------------
// template< typename type>
// class GlobalPlayerState : public State<type>
// {
// public:
// 
// 	CLASSFUNTION(GlobalPlayerState,type,Telegram);
// };
// 
// /// 追球
// //------------------------------------------------------------------------
// template< typename type>
// class ChaseBall : public State<type>
// {
// public:
// 
// 	CLASSFUNTION(ChaseBall,type,Telegram);
// 
// 	/*
// private:
// 
// 	ChaseBall(){}
// 
// public:
// 
// 	//this is a singleton
// 	static ChaseBall* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// 带球
// //------------------------------------------------------------------------
// template< typename type>
// class Dribble : public State<type>
// {
// public:
// 
// 	CLASSFUNTION(Dribble,type,Telegram);
// 
// 	/*
// private:
// 
// 	Dribble(){}
// 
// public:
// 
// 	//this is a singleton
// 	static Dribble* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player){}
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// 归位
// template< typename type>
// class ReturnToHomeRegion: public State<type>
// {
// public:
// 
// 	CLASSFUNTION(ReturnToHomeRegion,type,Telegram);
//  /*
// private:
// 
// 	ReturnToHomeRegion(){}
// 
// public:
// 
// 	//this is a singleton
// 	static ReturnToHomeRegion* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// 等待
// template< typename type>
// class Wait: public State<type>
// {
// public:
// 
// 	CLASSFUNTION(Wait,type,Telegram);
// 
// 	/*
// private:
// 
// 	Wait(){}
// 
// public:
// 
// 	//this is a singleton
// 	static Wait* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// 
// /// 跟踪球，防御
// template< typename type>
// class FollowBall :public State<type>
// {
// public:
// 
// 	CLASSFUNTION(FollowBall,type,Telegram);
// 
// 	/*
// private:
// 
// 	FollowBall() { }
// 
// public:
// 
// 	//this is a singleton
// 	static FollowBall* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
//   */
// };
// 
// /// 踢球
// template< typename type>
// class KickBall: public State<type>
// {
// public:
// 
// 	CLASSFUNTION(KickBall,type,Telegram);
// 
// 	/*
// private:
// 
// 	KickBall(){}
// 
// public:
// 
// 	//this is a singleton
// 	static KickBall* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player){}
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// 接球
// template< typename type>
// class ReceiveBall: public State<type>
// {
// public:
// 
// 	CLASSFUNTION(ReceiveBall,type,Telegram);
// 
// 	/*
// private:
// 
// 	ReceiveBall(){}
// 
// public:
// 
// 	//this is a singleton
// 	static ReceiveBall* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// 接应
// template< typename type>
// class SupportAttacker: public State<type>
// {
// public:
// 	CLASSFUNTION(SupportAttacker,type,Telegram);
// 	/*
// private:
// 
// 	SupportAttacker(){}
// 
// public:
// 
// 	//this is a singleton
// 	static SupportAttacker* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
