#pragma  once

#include "../../State.h"
#include "../../BaseEntity.h"

class State;

class CPeaceState : public State
{
public:
	// Enter a new State
	virtual void  Enter(CBaseEntity*);

	// Exectue
	virtual void  Exectue(CBaseEntity* );

	// Exit
	virtual void  Exit(CBaseEntity* );

	// Îö¹¹º¯Êý
	virtual	void  ~CPeaceState();
};