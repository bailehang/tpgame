#include "stdafx.h"
#include "EntityManager.h"

EntityManager::EntityManager()
{
	m_Entitymap.clear();
}

EntityManager::~EntityManager()
{
	m_Entitymap.clear();
}

void  EntityManager::RegEntity(CBaseEntity *entity)
{
	if ( !entity )  return ;

	m_Entitymap.insert( std::make_pair<long,CBaseEntity>(entity->GetID(),entity) );
}

void  EntityManager::RemoveEntity(CBaseEntity *entity)
{
	if( !entity )   return ;

	m_Entitymap.erase( entity );
}

CBaseEntity*  EntityManager::FindEntity(long id)
{
	EntityMap::const_iterator it = m_Entitymap.find( id );
	
	if ( it != m_Entitymap.end() )
	{
		return it->second;
	}
	return NULL;	
}