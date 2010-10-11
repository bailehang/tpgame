
#pragma  once 

#include "../../Public/MsgImpl.h"

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
	Ret Class::FunName(ENTITY* entity,const tagMessage&) { return false;}

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
	virtual bool OnMessage(entity_type*, const tagMessage&)=0;
};


/// 全局队员状态
CREATESTATE(GlobalPlayerState,tagMessage,FieldPlayer);

/// 追求状态
CREATESTATE(ChaseBall,tagMessage,FieldPlayer);

/// 带球状态
CREATESTATE(Dribble,tagMessage,FieldPlayer);

/// 回到出生
CREATESTATE(ReturnToHomeRegion,tagMessage,FieldPlayer);

/// 等待状态
CREATESTATE(Wait,tagMessage,FieldPlayer);

/// 跟踪球
CREATESTATE(FollowBall,tagMessage,FieldPlayer);

/// 踢球
CREATESTATE(KickBall,tagMessage,FieldPlayer);

///	接球状态
CREATESTATE(ReceiveBall,tagMessage,FieldPlayer);

///	接应状态
CREATESTATE(SupportAttacker,tagMessage,FieldPlayer);

/// 全局队员状态
CREATESTATE(GlobalKeeperState,tagMessage,GoalKeeper);

/// 守球门
CREATESTATE(TendGoal,tagMessage,GoalKeeper);

/// 截球
CREATESTATE(InterceptBall,tagMessage,GoalKeeper);

/// 归位
CREATESTATE(ReturnHome,tagMessage,GoalKeeper);

/// 球传回到赛场
CREATESTATE(PutBallBackInPlay,tagMessage,GoalKeeper);

/// 进攻状态
CREATESTATE(Attacking,tagMessage,SoccerTeam);

/// 防御状态
CREATESTATE(Defending,tagMessage,SoccerTeam);

/// 准备开球
CREATESTATE(PrepareForKickOff,tagMessage,SoccerTeam);

/// 边界球
CREATESTATE(Throw_In,tagMessage,SoccerTeam);

