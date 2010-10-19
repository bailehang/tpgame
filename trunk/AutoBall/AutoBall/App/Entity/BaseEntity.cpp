#include "Stdafx.h"
#include "BaseEntity.h"

int CBaseEntity::m_iNextValidID = 0;

CBaseEntity::CBaseEntity(long id)
{
	SetEntityType( default_entity_type );
	
	assert( id >= m_iNextValidID && "BaseEntity::ID is invalid ID");

	m_id = id ;
	m_dSize=0.0;
	m_bTag= false;
	m_vScal= Vector2D(1.0,1.0);
	m_iNextValidID = m_id + 1;
}