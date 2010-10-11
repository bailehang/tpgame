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

//----------------------------- Dispatch ---------------------------------
//  
//  see description in header
//------------------------------------------------------------------------
void MessageDispatcher::Discharge(CBaseEntity* pReceiver, const tagMessage& telegram)
{
	if (!pReceiver->HandleMessage(telegram))
	{
		//telegram could not be handled
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
	//first get current time
	double CurrentTime = TickCounter->GetCurrentFrame(); 

	//now peek at the queue to see if any telegrams need dispatching.
	//remove all telegrams from the front of the queue that have gone
	//past their sell by date
	while( !PriorityQ.empty() &&
		(PriorityQ.begin()->DispatchTime < CurrentTime) && 
		(PriorityQ.begin()->DispatchTime > 0) )
	{
		//read the telegram from the front of the queue
		const tagMessage& telegram = *PriorityQ.begin();

		//find the recipient
		CBaseEntity* pReceiver = GetInstObj(EntityManager).FindEntity(telegram.Receiver);

#ifdef SHOW_MESSAGING_INFO
		debug_con << "\nQueued telegram ready for dispatch: Sent to " 
			<< pReceiver->ID() << ". Msg is "<< telegram.Msg << "";
#endif

		//send the telegram to the recipient
		Discharge(pReceiver, telegram);

		//remove it from the queue
		PriorityQ.erase(PriorityQ.begin());
	}
}



