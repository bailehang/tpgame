#include "stdafx.h"
#include "EntityManager.h"

void  CEntityManager::RegEntityManager(CBaseGameEntity* Entity)
{
	m_EManager.insert( std::make_pair( Entity , Entity->GetEntityType() ) );
}

void  CEntityManager::RemoveEntity(CBaseGameEntity* Entity)
{
	m_EManager.erase( m_EManager.find(Entity) );
}

CBaseGameEntity*	CEntityManager::GetEntity( eEntityType& e )
{
// 	EntityIter iter = m_EManager.find( e );
// 	if ( iter != m_EManager.end() )
// 	{
// 		return iter->second;
// 	}
	return NULL;
}

void	CEntityManager::Update()
{
	for ( EntityIter iter = m_EManager.begin() ; iter != m_EManager.end(); iter ++ )
	{
		(iter->first)->Update();
	}
}