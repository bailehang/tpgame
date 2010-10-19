#include "Stdafx.h"
#include "MessageDispatcher.h"
#include "../Entity/BaseEntity.h"
#include "../Entity/EntityManager.h"
#include "../../Public/FrameCounter.h"
#include "../../Public/Singleton.h"

using std::set;

MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher instance; 

	return &instance;
}


void MessageDispatcher::Discharge(CBaseEntity* pReceiver, const tagMessage& telegram)
{
	if (!pReceiver->HandleMessage(telegram))
	{

	}
}

void MessageDispatcher::DispatchMsg(double       delay,
									int          sender,
									int          receiver,
									int          msg,
									void*        AdditionalInfo = NULL)
{

	CBaseEntity* pReceiver = GetInstObj(EntityManager).FindEntity(receiver);

	if (pReceiver == NULL)
	{
		return;
	}

	tagMessage telegram(0, sender, receiver, msg, AdditionalInfo);
                     
	if (delay <= 0.0)                                                        
	{
		Discharge(pReceiver, telegram);
	}
	else
	{
		LONG64 CurrentTime = timeGetTime(); 

		telegram.DispatchTime = CurrentTime + delay;
		PriorityQ.insert(telegram);   
	}
}

void MessageDispatcher::DispatchDelayedMessages()
{ 
	LONG64 CurrentTime = timeGetTime(); 

	while( !PriorityQ.empty() &&
		   (PriorityQ.begin()->DispatchTime < CurrentTime) && 
		   (PriorityQ.begin()->DispatchTime > 0) )
	{
		const tagMessage& telegram = *PriorityQ.begin();

		CBaseEntity* pReceiver = GetInstObj(EntityManager).FindEntity(telegram.Receiver);

		Discharge(pReceiver, telegram);

		PriorityQ.erase(PriorityQ.begin());
	}
}



