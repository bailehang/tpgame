#include "PlayerEntity.h"

extern CStateReg	g_state;
void CPlayerEntity::CPlayerEntity( eEntityType id  ) : CBaseEntity( id )
{
	m_pCurrentState = g_state.GetState( ent_Peace );
}
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

void CPlayerEntity::ChangeState( eState new_state )
{
	if ( new_state != m_pCurrentState->GetState() )
	{
		m_pCurrentState->Exit( this );

		m_pCurrentState = g_state.GetState( new_state ) ;
		m_pCurrentState->Enter( this );
	}
}  