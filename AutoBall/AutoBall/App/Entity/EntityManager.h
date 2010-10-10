/**
 *   @brief 实体管理类
 *			管理所有的实体类型，同时提供绑定和查找索引接口。
 *
 */

#pragma once 
#include "BaseEntity.h"

#include <vector>
#include <map>

using namespace std;

/// 实体Manager
class  CBaseEntity;
class  EntityManager
{
	
	typedef  std::map<long,CBaseEntity*>  EntityMap;

public:
	EntityManager(){ m_Entitymap.clear(); }
	~EntityManager(){}

	void  RegEntity(CBaseEntity* entity);

	void  RemoveEntity(CBaseEntity* entity);

	CBaseEntity*  FindEntity(long  id);

private:
	EntityMap	m_Entitymap;
};