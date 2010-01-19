#include "PlayerEntity.h"

void CPlayerEntity::Update()
{
	m_pCurrentState->Exectue( this );
}

void CPlayerEntity::ChangeState( State* new_state )
{
	if ( new_state != m_pCurrentState )
	{
		m_pCurrentState->Exit( this );

		m_pCurrentState = new_state ;
		m_pCurrentState->Enter( this );
	}
}