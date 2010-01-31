#pragma  once

#include "BaseDef.h"

template < class entity_type>
class CState
{
public:

	// Handle Event
	virtual	bool 	  HandleEvent( eEventAi& e , entity_type*) = 0;

	// return state
	virtual eStateAi  GetState() {	return  Ent_InValidState ;	}

	// Enter a new State
	virtual void  Enter(entity_type*)= 0;

	// Exectue
	virtual void  Exectue(entity_type* )=0;

	// Exit
	virtual void  Exit(entity_type* )=0;
	
	// 
	virtual	~CState();

};