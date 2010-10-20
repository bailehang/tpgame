
#pragma once 
#pragma warning (disable:4786)

#include <set>
#include <string>


#include "../../Public/MsgImpl.h"
#include "../../Public/Singleton.h"

class  CBaseEntity;
class  EntityManager;

class MessageDispatcher;

#define Dispatcher MsgDispatcher()

const double SEND_MSG_IMMEDIATELY = 0.0;
const int    NO_ADDITIONAL_INFO   = 0;
const int    SENDER_ID_IRRELEVANT = -1;

class MessageDispatcher
{
public:

	void DispatchMsg(double delay,
					 int    sender,
					 int    receiver,
					 int    msg,
					 void*  ExtraData);
public:

	void Discharge(CBaseEntity* pReceiver, const tagMessage& msg);

private:  

	std::set<tagMessage> PriorityQ;

};


inline MessageDispatcher*  MsgDispatcher()
{
	return	&GetInstObj(MessageDispatcher);
}
