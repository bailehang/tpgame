#include "PeaceState.h"

void  CPeaceState::Enter(CBaseEntity* Entity)
{
	CPlayerEntity* pEntity = static_cast<CPlayerEntity*>( Entity );

	if ( pEntity != NULL  && pEntity->GetState() == ent_Peace )
	{
		std::cout << " Entity  Enter Peace State " << std::endl;
	}
}

void  CPeaceState::Exectue(CBaseEntity* Entity)
{
	CPlayerEntity* pEntity = static_cast<CPlayerEntity*>( Entity );

	if ( pEntity != NULL  && pEntity->GetState() == ent_Peace )
	{
		std::cout << " Entity  Exectue Peace State " << std::endl;
	}
}

void  CPeaceState::Exit(CBaseEntity* Entity)
{
	CPlayerEntity* pEntity = static_cast<CPlayerEntity*>( Entity );

	if ( pEntity != NULL  && pEntity->GetState() == ent_Peace )
	{
		std::cout << " Entity  Exit Peace State " << std::endl;
	}
}
