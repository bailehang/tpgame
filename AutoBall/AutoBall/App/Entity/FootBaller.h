
#pragma  once 

#include "../../Public/Common/Autolist.h"
#include "../../Public/Singleton.h"
#include "../Script/StateMachineScript.h"
#include "../TimeCount.h"
#include "EntityManager.h"
#include "BasePlayer.h"

class FootBallTeam;
class FootBall;
class FootBall;
class Region;
class TimeCount;
template < class T>
class  StateMachine;
template < class T>
class  State;

/// 足球运动员
class FootBaller : public BasePlayer
{

public:

	FootBaller(FootBallTeam*    home_team,
				int			   home_region,
				State<FootBaller>* start_state,
				Vector2D	   heading,
				Vector2D       velocity,
				double         mass,
				double         max_force,
				double         max_speed,
				double         max_turn_rate,
				double         scale,
				player_role    role); 

	FootBaller(FootBallTeam*    home_team,
				int			   home_region,
				string		   State,
				Vector2D	   heading,
				Vector2D       velocity,
				double         mass,
				double         max_force,
				double         max_speed,
				double         max_turn_rate,
				double         scale,
				player_role    role);

	~FootBaller();

	void        Update();   

	void        Render();

	bool        OnMessage(const tagMessage& msg);

	StateMachine<FootBaller>* GetFSM()const{return m_pStateMachine;}

	StateMachineScript<FootBaller>* GetScriptFSM()const{return m_pStateMachineScript;}
								 
	/// 当处于kick的时候，下次带球的踢球准备时间是否已经到达
	bool        isReadyForNextKick()const{return m_pKickLimiter->isReadyOK();}

	long		GetScriptValue() ;

	void		SetScriptValue(long value){ m_ScriptValue = value;}

private:
	
	/// 当前脚本返回值
	long  m_ScriptValue;

	/// 状态机
	StateMachine<FootBaller>*  m_pStateMachine;

	/// 脚本状态机
	StateMachineScript<FootBaller>* m_pStateMachineScript;

	/// 限制有限次数的踢球次数
	TimeCount*                  m_pKickLimiter;

};

inline FootBaller*	GetExtraInfoField(const tagMessage* msg)	
{	
	return (FootBaller*)msg->ExtraInfo;
}

inline long   GetGlobalEntityID( int Revid )
{
	FootBaller*  player = static_cast<FootBaller*>( GetInstObj(EntityManager).FindEntity( Revid ) );

	if ( player != NULL )
	{
		return player->GetID();
	}

	return 0;
}