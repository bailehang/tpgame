#pragma once

#include "Ai/States.h"
#include "obj/Region.h"

namespace AI_SYSTEM
{
	typedef OPERATE_RESULT			ORESULT;
	typedef AI_SYSTEM::CRegion		Region;
	class CBaseAi
	{
	public:
		/*
		 *	初始化信息
		 */
		CBaseAi(void);

		~CBaseAi(void);

		virtual	 bool	Init( CObj_MoveShape	* pOwner);

		virtual  void	Term( void );

		/// 定时器封装操作
	public:

	
		/// 行为框架(主动行为)
	public:
		/*
		 *	主动行为响应,	主动行为响应， 不同状态的行为和平，战斗，逃跑，死亡，挂起
		 */
		virtual		ORESULT		OnActive(void);
		virtual		ORESULT		OnPeaceState();
		virtual		ORESULT		OnFightState();
		virtual		ORESULT		OnEscapeState();
		virtual		ORESULT		OnDeadState();
		virtual		ORESULT		OnHoldOnState();
		
		/// 行为框架(被动行为)
	public:
		/*
		 *	 当被攻击的时候
		 */
		virtual		ORESULT		WhenBeenHurted();
		virtual		ORESULT		WhenBeenKilled()

		/// 公共行为
	public:

		virtual		ORESULT		Run( void );
		virtual		ORESULT		Jump( void );
		virtual		ORESULT		Stop( void );
		virtual		ORESULT		Stall( void );
		virtual		ORESULT		Move ( const Region *pTar );
		virtual		ORESULT		MoveTo( long lDesX , long  lDesY );
		virtual		ORESULT		RandomRun( void );
		virtual		ORESULT		RunByRoad( void );
		virtual		ORESULT		Obj_Move(  const Region *pTar );

		//	virtual		ORESULT	Obj_UseSkill( long  SkillID ,..);


		/// 事件相关
	public:

		/*
		 *	全部AI逻辑
		 */
	public:
		virtual		bool	Logic()		{	return	m_Aistates.Logic(); }
		//virtual		

	private:
		CObj_MoveShape		m_pOwner;
		CStates				m_Aistates;
	};
}