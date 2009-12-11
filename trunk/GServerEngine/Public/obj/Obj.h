#pragma once
#include "Region.h"


class  CGUID;
class CObj
{
	/// 对象类型
	enum  OBJ_TYPE
	{
		OBJ_TYPE_INVALID,			// 无效
		OBJ_TYPE_PLAYER,			// 玩家
		OBJ_NPC,					// NPC
		OBJ_TYPE_MONSTER,			// 可以战斗的怪物
		OBJ_TYPE_PET,				// 宠物		
		/// 对于有新的类型即可增加
	};

	/// 状态
	enum  STATE
	{	
		STATE_PEACE,				// 和平状态
		STATE_FIGHT,				// 战斗状态
		STATE_DIED,					// 死亡状态
		STATE_FEIGNDEATH,			// 假死状态
		STATE_HANGUP,				// 挂起状态
		STATE_DELETE,				// 删除状态
		/// 不足可以增加
	};

public:

	CObj(void);

	~CObj(void);

	/*
	 *	定义成员函数
	 */
public:

	virtual  void		Init()	;
	virtual	 void		Clear() ;
	virtual	 void		SetObjType( OBJ_TYPE  obj ) {	m_obj = obj ;		}
	virtual	 OBJ_TYPE	GetObjType() const			{	return   m_obj;		}
	virtual	 void		SetState( STATE  state )	{	m_state = state ;	}
	virtual	 STATE		GetState() const			{	return   m_state;	}	
	virtual	 CGUID&		GetGUID() const				{	return	 m_GUID;	}
	virtual	 bool		StarAi( int	uTimer =0 );
	

private:
	AI_SYSTEM::CRegion		m_Pos;
	CGUID					m_GUID;
	OBJ_TYPE				m_obj;
	STATE					m_state;

};
