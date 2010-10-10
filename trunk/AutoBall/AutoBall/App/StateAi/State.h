
#pragma  once 

#include "../Telegram.h"

class SoccerTeam;
class GoalKeeper;
class SoccerPitch;
class FieldPlayer;

#define  DIS(s)		#s
#define  DISTROY(s) DIS(s)

#define  EntityFun(Ret,FunName,Entity)\
	Ret  FunName(Entity* entity)
#define  MessageFun(Ret,FunName,Entity,Msg)\
	Ret  FunName(Entity* entity,const Msg&)

#define  EmptyFun(Ret,Class,FunName,ENTITY)\
	Ret Class::FunName(ENTITY* type) {}

#define  EmptyMsg(Ret,Class,FunName,ENTITY)\
	Ret Class::FunName(ENTITY* entity,const Telegram&) { return false;}

#define  CLASSFUNTION(CLASS,ENTITY,MSG)\
	   EntityFun(void,Enter,ENTITY);\
	   EntityFun(void,Execute,ENTITY);\
	   EntityFun(void,Exit,ENTITY);\
	   MessageFun(bool,OnMessage,ENTITY,MSG)

#define  CREATESTATE(CLASSNAME,MSG,Player) \
	class CLASSNAME : public State<Player> \
	{ \
	public:\
		CLASSFUNTION(GlobalPlayerState,Player,MSG);\
	}

template <class entity_type>
class State
{
public:

	virtual ~State()	{}

	//this will execute when the state is entered
	virtual void Enter(entity_type*)=0;

	//this is the states normal update function
	virtual void Execute(entity_type*)=0;

	//this will execute when the state is exited. 
	virtual void Exit(entity_type*)=0;

	//this executes if the agent receives a message from the 
	//message dispatcher
	virtual bool OnMessage(entity_type*, const Telegram&)=0;
};


/// 全局队员状态
CREATESTATE(GlobalPlayerState,Telegram,FieldPlayer);

/// 追求状态
CREATESTATE(ChaseBall,Telegram,FieldPlayer);

/// 带球状态
CREATESTATE(Dribble,Telegram,FieldPlayer);

/// 回到出生
CREATESTATE(ReturnToHomeRegion,Telegram,FieldPlayer);

/// 等待状态
CREATESTATE(Wait,Telegram,FieldPlayer);

/// 跟踪球
CREATESTATE(FollowBall,Telegram,FieldPlayer);

/// 踢球
CREATESTATE(KickBall,Telegram,FieldPlayer);

///	接球状态
CREATESTATE(ReceiveBall,Telegram,FieldPlayer);

///	接应状态
CREATESTATE(SupportAttacker,Telegram,FieldPlayer);

/// 全局队员状态
CREATESTATE(GlobalKeeperState,Telegram,GoalKeeper);

/// 守球门
CREATESTATE(TendGoal,Telegram,GoalKeeper);

/// 截球
CREATESTATE(InterceptBall,Telegram,GoalKeeper);

/// 归位
CREATESTATE(ReturnHome,Telegram,GoalKeeper);

/// 球传回到赛场
CREATESTATE(PutBallBackInPlay,Telegram,GoalKeeper);

/// 进攻状态
CREATESTATE(Attacking,Telegram,SoccerTeam);

/// 防御状态
CREATESTATE(Defending,Telegram,SoccerTeam);

/// 准备开球
CREATESTATE(PrepareForKickOff,Telegram,SoccerTeam);

/// 边界球
CREATESTATE(Throw_In,Telegram,SoccerTeam);

