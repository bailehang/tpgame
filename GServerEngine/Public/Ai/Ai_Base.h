#pragma once

namespace AI_SYSTEM
{
	
	typedef OPERATE_RESULT ORESULT;
	class CBaseAi
	{
	public:
		CBaseAi(void);

		~CBaseAi(void);
		
		virtual	 bool	Init( CObj_MoveShape	* pOwner);

		virtual  void	Term( void );

	public:
	
		/// 定时器封装操作
	public:

		/// 行为框架(主动行为)
	public:

		/// 公共行为
	public:

		/// 事件相关
	public:
		

	private:
		CObj_MoveShape		m_pOwner;

	};
}