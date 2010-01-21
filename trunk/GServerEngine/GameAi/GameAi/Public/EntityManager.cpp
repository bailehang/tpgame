#include "stdafx.h"
#include "EntityManager.h"

void  CEntityManager::RegEntityManager(CBaseGameEntity* Entity)
{
	m_EManager.insert( std::make_pair( Entity->GetEntityType() , Entity) );
}

void  CEntityManager::RemoveEntity(CBaseGameEntity* Entity)
{
	m_EManager.erase( m_EManager.find(Entity->GetEntityType()) );
}

CBaseGameEntity*	CEntityManager::GetEntity( eEntityType& e )
{
	EntityIter iter = m_EManager.find( e );
	if ( iter != m_EManager.end() )
	{
		return iter->second;
	}
	return NULL;
}