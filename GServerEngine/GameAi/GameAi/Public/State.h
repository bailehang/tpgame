#pragma  once

#include "BaseDef.h"
#include "BaseGameEntity.h"

class CState
{
public:

	// Handle Event
	virtual	bool 	  HandleEvent( eEventAi e , CBaseGameEntity*) ;

	// return state
	virtual eStateAi  GetState() {	return  Ent_InValidState ;	}

	// Enter a new State
	virtual void  Enter(CBaseGameEntity*)=0;

	// Exectue
	virtual void  Execute(CBaseGameEntity*)=0;

	// Exit
	virtual void  Exit(CBaseGameEntity* )=0;
	
	// 
	virtual	~CState();

};