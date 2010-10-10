
#pragma  once 

#include "../../Public/Common/Autolist.h"
#include "../Regulator.h"

class SoccerTeam;
class SoccerPitch;
class SoccerBall;
class Region;
class Regulator;
template < class T>
class  StateMachine;
template < class T>
class  State;

/// 足球运动员
class FieldPlayer : public PlayerBase
{

public:

	FieldPlayer(SoccerTeam*    home_team,
				int			   home_region,
				State<FieldPlayer>* start_state,
				Vector2D	   heading,
				Vector2D       velocity,
				double         mass,
				double         max_force,
				double         max_speed,
				double         max_turn_rate,
				double         scale,
				player_role    role);   

	~FieldPlayer();

	void        Update();   

	void        Render();

	bool        HandleMessage(const Telegram& msg);

	StateMachine<FieldPlayer>* GetFSM()const{return m_pStateMachine;}
								 
	/// 当处于kick的时候，下次带球的踢球准备时间是否已经到达
	bool        isReadyForNextKick()const{return m_pKickLimiter->isReady();}

private:

	/// 状态机
	StateMachine<FieldPlayer>*  m_pStateMachine;

	/// 限制有限次数的踢球次数
	Regulator*                  m_pKickLimiter;


};