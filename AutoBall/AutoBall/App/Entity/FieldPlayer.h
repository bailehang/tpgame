
#pragma  once 

#include "../../Public/Common/Autolist.h"
#include "MoveEntity.h"

class SoccerTeam;
class SoccerPitch;
class SoccerBall;
class SteeringBehaviors;
class Region;

template < class T>
class  StateMachine;
template < class T>
class  State;

/// ◊„«Ú‘À∂Ø‘±
class FieldPlayer : public PlayerBase
{
private:

	//an instance of the state machine class
	StateMachine<FieldPlayer>*  m_pStateMachine;

	//limits the number of kicks a player may take per second
	Regulator*                  m_pKickLimiter;


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

	//call this to update the player's position and orientation
	void        Update();   

	void        Render();

	bool        HandleMessage(const Telegram& msg);

	StateMachine<FieldPlayer>* GetFSM()const{return m_pStateMachine;}

	bool        isReadyForNextKick()const{return m_pKickLimiter->isReady();}

};