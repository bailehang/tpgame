#pragma once
#include "Region.h"
#include "../GUID.h"
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
	virtual	 void		SetState( CStates  state )	{	m_state = state ;	}
	virtual	 CStates	GetState() const			{	return   m_state;	}	
	virtual	 CGUID&		GetGUID()					{	return	 m_GUID;	}
	virtual	 bool		StarAi( int	uTimer =0 );

	/*
	 *	ai调用
	 */
public:
		

private:
	AI_SYSTEM::CRegion		m_Pos;
	CGUID					m_GUID;
	OBJ_TYPE				m_obj;
	CStates					m_state;

};
