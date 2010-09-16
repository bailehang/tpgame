
#pragma  once 

#include <vector>
#include "../Render/Vector2D.h"
#include "SupportSpotCalculator.h"

class Goal;
class PlayerBase;
class FieldPlayer;
class SoccerPitch;
class GoalKeeper;
class SupportSpotCalculator;
template<class T>
class StateMachine;
class SupportSpotCalculator;
	  
class SoccerTeam 
{
public:

	enum team_color {blue, red};		  /// 球队类型

private:

	//an instance of the state machine class
	StateMachine<SoccerTeam>*  m_pStateMachine;///> 当前队伍的状态机

	//the team must know its own color!
	team_color                m_Color;

	//pointers to the team members
	std::vector<PlayerBase*>  m_Players;	   ///> 队伍所有的成员

	//a pointer to the soccer pitch
	SoccerPitch*              m_pPitch;		   ///> 指向的球场指针

	//pointers to the goals
	Goal*                     m_pOpponentsGoal;///> 对方球门
	Goal*                     m_pHomeGoal;	   ///> 自己球门

	//a pointer to the opposing team
	SoccerTeam*               m_pOpponents;	   ///> 敌对队伍指针

	//pointers to 'key' players
	PlayerBase*               m_pControllingPlayer;	 ///> 控球队员
	PlayerBase*               m_pSupportingPlayer;	 ///> 接应队员
	PlayerBase*               m_pReceivingPlayer;	 ///> 指向接球队员
	PlayerBase*               m_pPlayerClosestToBall;///> 指向离球最近的队员

	//the squared distance the closest player is from the ball
	double                     m_dDistSqToBallOfClosestPlayer;///> 离最近球员最近的距离

	//players use this to determine strategic positions on the playing field
	SupportSpotCalculator*    m_pSupportSpotCalc;	 ///>  用来决策队员的行为类


	//creates all the players for this team
	void CreatePlayers();

	//called each frame. Sets m_pClosestPlayerToBall to point to the player
	//closest to the ball. 
	void CalculateClosestPlayerToBall();			///> 每一帧计算当前离球最近的球员

	bool m_IsChase; 
	bool m_IsThrowIn;

public:

	SoccerTeam(Goal*        home_goal,
		Goal*        opponents_goal,
		SoccerPitch* pitch,
		team_color   color);

	~SoccerTeam();

	//the usual suspects
	void        Render()const;
	void        Update();

	//calling this changes the state of all field players to that of 
	//ReturnToHomeRegion. Mainly used when a goal keeper has
	//possession
	void        ReturnAllFieldPlayersToHome()const;

	//returns true if player has a clean shot at the goal and sets ShotTarget
	//to a normalized vector pointing in the direction the shot should be
	//made. Else returns false and sets heading to a zero vector
	///  计算是否能射门进球
	bool        CanShoot(Vector2D  BallPos,
		double     power, 
		Vector2D& ShotTarget = Vector2D())const;

	//The best pass is considered to be the pass that cannot be intercepted 
	//by an opponent and that is as far forward of the receiver as possible  
	//If a pass is found, the receiver's address is returned in the 
	//reference, 'receiver' and the position the pass will be made to is 
	//returned in the  reference 'PassTarget'
	/// 队员调用次方法，决定是否能够传球，如果可以，那么谁是最佳的传球对象
	/// 什么位置又是最佳的传球位置
	bool        FindPass(const PlayerBase*const passer,
		PlayerBase*&           receiver,
		Vector2D&              PassTarget,
		double                  power,
		double                  MinPassingDistance)const;

	//Three potential passes are calculated. One directly toward the receiver's
	//current position and two that are the tangents from the ball position
	//to the circle of radius 'range' from the receiver.
	//These passes are then tested to see if they can be intercepted by an
	//opponent and to make sure they terminate within the playing area. If
	//all the passes are invalidated the function returns false. Otherwise
	//the function returns the pass that takes the ball closest to the 
	//opponent's goal area.
	/// 给定一个传球者和一个接应者，这个方法测试接球者附近的若干不同位置，
	/// 判断是否可以安全的传球，如果可以传球，
	/// 那么该方法把最佳传球保存在参数PassTarget中
	bool        GetBestPassToReceiver(const PlayerBase* const passer,
		const PlayerBase* const receiver,
		Vector2D& PassTarget,
		const double power)const;

	//test if a pass from positions 'from' to 'target' kicked with force 
	//'PassingForce'can be intercepted by an opposing player
	/// 计算FORM到target对方球员opp是否可以夺走足球
	bool        isPassSafeFromOpponent(Vector2D    from,
		Vector2D    target,
		const PlayerBase* const receiver,
		const PlayerBase* const opp,
		double       PassingForce)const;

	//tests a pass from position 'from' to position 'target' against each member
	//of the opposing team. Returns true if the pass can be made without
	//getting intercepted
	/// 计算FORM到target对方所有的球员是否可以夺走足球
	bool        isPassSafeFromAllOpponents(Vector2D from,
		Vector2D target,
		const PlayerBase* const receiver,
		double     PassingForce)const;

	//returns true if there is an opponent within radius of position
	bool        isOpponentWithinRadius(Vector2D pos, double rad);

	//this tests to see if a pass is possible between the requester and
	//the controlling player. If it is possible a message is sent to the
	//controlling player to pass the ball asap.
	void        RequestPass(FieldPlayer* requester)const;

	//calculates the best supporting position and finds the most appropriate
	//attacker to travel to the spot
	/// 计算最佳传球位置，并查找最合适的对方队员前往这个地点
	PlayerBase* DetermineBestSupportingAttacker();


	const std::vector<PlayerBase*>& Members()const{return m_Players;}  

	StateMachine<SoccerTeam>* GetFSM()const{return m_pStateMachine;}

	Goal*const           HomeGoal()const{return m_pHomeGoal;}
	Goal*const           OpponentsGoal()const{return m_pOpponentsGoal;}

	SoccerPitch*const    Pitch()const{return m_pPitch;}           

	SoccerTeam*const     Opponents()const{return m_pOpponents;}
	void                 SetOpponents(SoccerTeam* opps){m_pOpponents = opps;}

	team_color           Color()const{return m_Color;}

	void                 SetPlayerClosestToBall(PlayerBase* plyr){m_pPlayerClosestToBall=plyr;}
	PlayerBase*          PlayerClosestToBall()const{return m_pPlayerClosestToBall;}

	double               ClosestDistToBallSq()const{return m_dDistSqToBallOfClosestPlayer;}

	Vector2D             GetSupportSpot()const{return m_pSupportSpotCalc->GetBestSupportingSpot();}

	PlayerBase*          SupportingPlayer()const{return m_pSupportingPlayer;}
	void                 SetSupportingPlayer(PlayerBase* plyr){m_pSupportingPlayer = plyr;}

	PlayerBase*          Receiver()const{return m_pReceivingPlayer;}
	void                 SetReceiver(PlayerBase* plyr){m_pReceivingPlayer = plyr;}

	PlayerBase*          ControllingPlayer()const{return m_pControllingPlayer;}
	void                 SetControllingPlayer(PlayerBase* plyr)
	{
		m_pControllingPlayer = plyr;

		//rub it in the opponents faces!
		Opponents()->LostControl();
	}


	bool  InControl()const{if(m_pControllingPlayer)return true; else return false;}
	void  LostControl(){m_pControllingPlayer = NULL;}

	PlayerBase*  GetPlayerFromID(int id)const;


	void SetPlayerHomeRegion(int plyr, int region)const;

	void DetermineBestSupportingPosition()const
	{
		m_pSupportSpotCalc->DetermineBestSupportingPosition();
	}

	void UpdateTargetsOfWaitingPlayers()const;

	//returns false if any of the team are not located within their home region
	bool AllPlayersAtHome()const;

	std::string Name() const 
	{
		if (m_Color == blue) 
			return "Blue"; 
		return "Red";
	}

	bool  IsChaseBall() const	   { return m_IsChase; }
	void  SetChaseBall(bool chase) { m_IsChase = chase;}

	bool  IsThrowIn() const	   { return m_IsThrowIn; }
	void  SetThrowIn(bool throwin) { m_IsThrowIn = throwin;}

};