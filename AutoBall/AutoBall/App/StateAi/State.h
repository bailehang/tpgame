
#pragma  once 

#include "../Telegram.h"

#define  DIS(s)		#s
#define  DISTROY(s) DIS(s)

#define  CREATECLASS(NAME) \
	private:\
	NAME(){}\		

#define  EntityFun(Ret,FunName,Entity)\
	Ret  FunName(Entity* entity)
#define  MessageFun(Ret,FunName,Entity,Msg)\
	Ret  FunName(Entity* entity,const Msg&)

#define  EmptyFun(Ret,Class,FunName,ENTITY)\
	Ret Class::FunName(ENTITY* type) {}

#define  EmptyMsg(Ret,Class,FunName,ENTITY)\
	Ret Class::FunName(ENTITY* entity,const Telegram&) { return false;}

#define  CLASSFUNTION(CLASS,ENTITY,MSG)\
	public:\
	   EntityFun(void,Enter,ENTITY);\
	   EntityFun(void,Execute,ENTITY);\
	   EntityFun(void,Exit,ENTITY);\
	   MessageFun(bool,OnMessage,ENTITY,MSG);

#define  CREATESTATE(CLASSNAME,MSG) \
	class CLASSNAME : public State<FieldPlayer> \
	{ \
	public:\
		CLASSFUNTION(GlobalPlayerState,FieldPlayer,MSG);\
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
