#include "FleeState.h"

void  CFleeState::Enter(CBaseEntity* Entity)
{
	CPlayerEntity* pEntity = static_cast<CPlayerEntity*>( Entity );

	if ( pEntity != NULL  && pEntity->GetState() == ent_Flee )
	{
		std::cout << " Entity  Enter Flee State " << std::endl;
	}
}

void  CFleeState::Exectue(CBaseEntity* Entity)
{
	CPlayerEntity* pEntity = static_cast<CPlayerEntity*>( Entity );

	if ( pEntity != NULL  && pEntity->GetState() == ent_Flee )
	{
		std::cout << " Entity  Exectue Flee State " << std::endl;
	}
}

void  CFleeState::Exit(CBaseEntity* Entity)
{
	CPlayerEntity* pEntity = static_cast<CPlayerEntity*>( Entity );

	if ( pEntity != NULL  && pEntity->GetState() == ent_Flee )
	{
		std::cout << " Entity  Exit Flee State " << std::endl;
	}
}