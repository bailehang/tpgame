

#pragma once 
#include "BaseEntity.h"
#include <vector>
#include <map>

using namespace std;


class  EntityManager
{
	
	typedef  std::map<long,CBaseEntity*>  EntityMap;

public:
	EntityManager();
	~EntityManager();

	void  RegEntity(CBaseEntity* entity);

	void  RemoveEntity(CBaseEntity* entity);

	CBaseEntity*  FindEntity(long  id);

private:
	EntityMap	m_Entitymap;
}