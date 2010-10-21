
#pragma once 
#pragma warning (disable:4786)

#include <set>
#include <string>


#include "../../Public/MsgImpl.h"
#include "../../Public/Singleton.h"
#include "../../Render/Vector2D.h"

class  CBaseEntity;
class  EntityManager;

class MessageDispatcher;

#define Dispatcher MsgDispatcher()

const long SEND_MSG_IMMEDIATELY = 0;

class MessageDispatcher
{
public:

	void DispatchMsg(int    sender,
					 int    receiver,
					 int    msg,
					 void*  Data);
	void DispatchVctMsg(int     sender,
					 int		receiver,
					 int        msg,
					 Vector2D   Data);
private:

	void Discharge(CBaseEntity* pReceiver, const tagMessage& msg);

};


inline MessageDispatcher*  MsgDispatcher()
{
	return	&GetInstObj(MessageDispatcher);
}


inline void DispatchVctMessage(int     sender,
						  int		receiver,
					      int        msg,
					      Vector2D   Data)
{
	  Dispatcher->DispatchVctMsg(sender,receiver,msg,Data);
}