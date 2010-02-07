#pragma  once

#include "../../Public/State.h"

class  CPeaceState : public CState
{

	//typedef  entity_name    Entity;

public:

	void Enter(CBaseGameEntity* );
		 
	void Execute(CBaseGameEntity* );

	void Exit(CBaseGameEntity* );

	bool HandleEvent( eEventAi e , CBaseGameEntity*) ;

	// return state
	eStateAi GetState()	{	return  Ent_PeaceState ;	}

private:
	

};

//#include "PeaceState.cpp"