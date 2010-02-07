#pragma  once

#include "../../Public/State.h"

class  CFollowState : public CState
{

//	typedef  entity_name    Entity;

public:

	virtual void Enter(CBaseGameEntity* );

	virtual void Execute(CBaseGameEntity* );

	virtual void Exit(CBaseGameEntity* );

	virtual	bool HandleEvent( eEventAi e , CBaseGameEntity*) ;

	// return state
	virtual eStateAi GetState()	{	return  Ent_FollowState ;	}

private:


};