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
#ifdef SHOW_MESSAGING_INFO
		debug_con << "Message not handled" << "";
#endif
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
#ifdef SHOW_MESSAGING_INFO
		debug_con << "\nWarning! No Receiver with ID of " << receiver << " found" << "";
#endif
		return;
	}

	tagMessage telegram(0, sender, receiver, msg, AdditionalInfo);
                     
	if (delay <= 0.0)                                                        
	{
#ifdef SHOW_MESSAGING_INFO
		debug_con << "\nTelegram dispatched at time: " << TickCounter->GetCurrentFrame()
			<< " by " << sender << " for " << receiver 
			<< ". Msg is " << msg << "";
#endif
		Discharge(pReceiver, telegram);
	}
	else
	{
		double CurrentTime = TickCounter->GetCurrentFrame(); 

		telegram.DispatchTime = CurrentTime + delay;

		PriorityQ.insert(telegram);   

#ifdef SHOW_MESSAGING_INFO
		debug_con << "\nDelayed telegram from " << sender << " recorded at time " 
			<< TickCounter->GetCurrentFrame() << " for " << receiver
			<< ". Msg is " << msg << "";
#endif
	}
}

void MessageDispatcher::DispatchDelayedMessages()
{ 
	double CurrentTime = TickCounter->GetCurrentFrame(); 

	while( !PriorityQ.empty() &&
		   (PriorityQ.begin()->DispatchTime < CurrentTime) && 
		   (PriorityQ.begin()->DispatchTime > 0) )
	{
		const tagMessage& telegram = *PriorityQ.begin();

		CBaseEntity* pReceiver = GetInstObj(EntityManager).FindEntity(telegram.Receiver);

#ifdef SHOW_MESSAGING_INFO
		debug_con << "\nQueued telegram ready for dispatch: Sent to " 
			<< pReceiver->ID() << ". Msg is "<< telegram.Msg << "";
#endif

		Discharge(pReceiver, telegram);

		PriorityQ.erase(PriorityQ.begin());
	}
}



