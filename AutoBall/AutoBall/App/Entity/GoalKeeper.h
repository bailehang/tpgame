
#pragma  once

#include "../../Public/Common/Autolist.h"
#include "MoveEntity.h"

class SoccerTeam;
class SoccerPitch;
class SoccerBall;
class SteeringBehaviors;
class Region;
class PlayerBase;
template < class T>
class  State;
template < class T>
class  StateMachine;

/// 封装守门员类
class  GoalKeeper : public PlayerBase
{

public:
	GoalKeeper(SoccerTeam*        home_team,
			   int                home_region,
			   State<GoalKeeper>* start_state,
			   Vector2D           heading,
			   Vector2D           velocity,
			   double             mass,
			   double             max_force,
			   double             max_speed,
			   double             max_turn_rate,
		       double             scale);

	~GoalKeeper() { SAFE_DELETE(m_pStateMachine); }

	//these must be implemented
	void        Update();
	void        Render();
	bool        HandleMessage(const Telegram& msg);
	void		Write(std::ostream&  os){}
	void		Read (std::ifstream& is){}

	bool        BallWithinRangeForIntercept()const;

	//returns true if the keeper has ventured too far away from the goalmouth
	bool        TooFarFromGoalMouth()const;

	Vector2D    GetRearInterposeTarget()const;

	StateMachine<GoalKeeper>* GetFSM()const{return m_pStateMachine;}


	Vector2D    LookAt()const		 {return m_vLookAt;}
	void        SetLookAt(Vector2D v){m_vLookAt=v;}

private:
	
	StateMachine<GoalKeeper>*  m_pStateMachine;

	Vector2D   m_vLookAt;

};