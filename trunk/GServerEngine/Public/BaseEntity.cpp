#include "StdAfx.h"
#include "BaseEntity.h"


long CBaseEntity::m_iNextValidId = 0;


void  CBaseEntity::SetId(int val)
{
	assert( val>0 && val <= m_iNextValidId);

	m_id = val;

	m_iNextValidId = m_id + 1;
}