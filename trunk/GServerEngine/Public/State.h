
#pragma  once 

enum  eAction
{
	ent_Flee,
	ent_Peace,
};

class State
{
public:
	// Enter a new State
	virtual void  Enter(CBaseEntity*)= 0;

	// Exectue
	virtual void  Exectue(CBaseEntity* )=0;

	// Exit
	virtual void  Exit(CBaseEntity* )=0;
};