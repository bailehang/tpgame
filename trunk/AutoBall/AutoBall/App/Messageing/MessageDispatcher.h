
#pragma once 
#pragma warning (disable:4786)

#include <set>
#include <string>


#include "../../Public/MsgImpl.h"


class  CBaseEntity;
class  EntityManager;

#define Dispatcher MessageDispatcher::Instance()

const double SEND_MSG_IMMEDIATELY = 0.0;
const int    NO_ADDITIONAL_INFO   = 0;
const int    SENDER_ID_IRRELEVANT = -1;


class MessageDispatcher
{
private:  

	std::set<tagMessage> PriorityQ;

	void Discharge(CBaseEntity* pReceiver, const tagMessage& msg);

	MessageDispatcher(){}

	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator=(const MessageDispatcher&);

public:

	static MessageDispatcher* Instance();

	void DispatchMsg(double delay,
					 int    sender,
					 int    receiver,
					 int    msg,
					 void*  ExtraInfo);

	void DispatchDelayedMessages();
};


inline MessageDispatcher*  MsgDispatcher()
{
	return	MessageDispatcher::Instance();
}
