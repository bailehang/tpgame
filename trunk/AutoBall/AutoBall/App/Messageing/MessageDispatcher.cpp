#include "Stdafx.h"
#include "MessageDispatcher.h"
#include "../Entity/BaseEntity.h"
#include "../Entity/EntityManager.h"
#include "../../Public/Singleton.h"

using std::set;




void MessageDispatcher::Discharge(CBaseEntity* pReceiver, const tagMessage& telegram)
{
	if (!pReceiver->OnMessage(telegram))
	{

	}
}

void MessageDispatcher::DispatchMsg(double       delay,
									int          sender,
									int          receiver,
									int          msg,
									void*        ExtraData = NULL)
{

	CBaseEntity* pReceiver = GetInstObj(EntityManager).FindEntity(receiver);

	if (pReceiver != NULL)
	{
		Discharge(pReceiver, tagMessage(0, sender, receiver, msg, ExtraData));
		return;
	}                  
	
}


