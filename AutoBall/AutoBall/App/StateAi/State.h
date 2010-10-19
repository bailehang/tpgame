
#pragma  once 

#include "../../Public/MsgImpl.h"

class FootBallTeam;
class GoalKeeper;
class FootBall;
class FootBaller;

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

template <class entity>
class State
{
public:

	virtual ~State()	{}

	virtual void Enter(entity*)=0;

	virtual void Execute(entity*)=0;

	virtual void Exit(entity*)=0;

	virtual bool OnMessage(entity*, const tagMessage&)=0;
};


/// 全局队员状态
CREATESTATE(GlobalPlayerState,tagMessage,FootBaller);

/// 追求状态
CREATESTATE(ChaseBall,tagMessage,FootBaller);

/// 带球状态
CREATESTATE(Dribble,tagMessage,FootBaller);

/// 回到出生
CREATESTATE(ReturnToHomeRegion,tagMessage,FootBaller);

/// 等待状态
CREATESTATE(Wait,tagMessage,FootBaller);

/// 跟踪球
CREATESTATE(FollowBall,tagMessage,FootBaller);

/// 踢球
CREATESTATE(KickBall,tagMessage,FootBaller);

///	接球状态
CREATESTATE(ReceiveBall,tagMessage,FootBaller);

///	接应状态
CREATESTATE(SupportAttacker,tagMessage,FootBaller);

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
CREATESTATE(Attacking,tagMessage,FootBallTeam);

/// 防御状态
CREATESTATE(Defending,tagMessage,FootBallTeam);

/// 准备开球
CREATESTATE(PrepareForKickOff,tagMessage,FootBallTeam);

/// 边界球
CREATESTATE(Throw_In,tagMessage,FootBallTeam);

