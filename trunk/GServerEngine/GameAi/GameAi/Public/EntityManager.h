#pragma once

#include "BaseDef.h"
#include "BaseGameEntity.h"

class   CEntityManager
{
	typedef std::map<  CBaseGameEntity* ,eEntityType >  EntityManager;
	typedef EntityManager::iterator						EntityIter;

private:
	// 实体管理器
	EntityManager		m_EManager;

	// 单件函数 定义私有函数
public:
	CEntityManager();
	~CEntityManager();
	CEntityManager(const CEntityManager&);
	CEntityManager& operator=(const CEntityManager&);

	// 成员函数
public:

	void	RegEntityManager(CBaseGameEntity* Entity);

	CBaseGameEntity*	GetEntity( eEntityType& e );

	void	RemoveEntity( CBaseGameEntity*    Entity);

	void	Update();
};