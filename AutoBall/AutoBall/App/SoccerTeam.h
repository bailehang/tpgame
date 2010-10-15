
#pragma  once 

#include <vector>
#include "../Render/Vector2D.h"
#include "SupportSpotCalculator.h"
#include "Goal.h"

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

public:

	SoccerTeam(Goal*  home_goal,
			   Goal*  opponents_goal,
			   SoccerPitch* pitch,
			   team_color   color);

	~SoccerTeam();

	void        Render()const;
	void        Update();

	/// 让所有的队员归位
	void        ReturnAllFieldPlayersToHome()const;

	///  计算是否能射门进球
	bool        CanShoot(Vector2D  BallPos,
						 double    power, 
						 Vector2D& ShotTarget = Vector2D())const;

	///
	/// 队员调用次方法，决定是否能够传球，如果可以，那么谁是最佳的传球对象
	/// 什么位置又是最佳的传球位置
	/// 
	bool        FindPass(const PlayerBase*const passer,
						 PlayerBase*&  receiver,
						 Vector2D&     PassTarget,
						 double        power,
						 double        MinPassingDistance)const;

	/// 给定一个传球者和一个接应者，这个方法测试接球者附近的若干不同位置，
	/// 判断是否可以安全的传球，如果可以传球，
	/// 那么该方法把最佳传球保存在参数PassTarget中
	bool        GetBestPassToReceiver(const PlayerBase* const passer,
									  const PlayerBase* const receiver,
									  Vector2D& PassTarget,
									  const double power)const;

	/// 计算FORM到target对方球员opp是否可以夺走足球
	bool        isPassSafeFromOpponent( Vector2D    from,
										Vector2D    target,
										const PlayerBase* const receiver,
										const PlayerBase* const opp,
										double       PassingForce)const;

	/// 计算FORM到target对方所有的球员是否可以夺走足球
	bool        isPassSafeFromAllOpponents(Vector2D from,
						Vector2D target,
						const PlayerBase* const receiver,
						double     PassingForce)const;

	/// 对方球员进行队员的防御范围
	bool        isOpponentWithinRadius(Vector2D pos, double rad);

	/// 请求控球队员传球
	void        RequestPass(FieldPlayer* requester)const;

	/// 计算最佳传球位置，并查找最合适的对方队员前往这个地点
	PlayerBase* DetermineBestSupportingAttacker();

	/// 得到所有的队内成员
	const std::vector<PlayerBase*>& Members()const{return m_Players;}  
	StateMachine<SoccerTeam>* GetFSM()const {return m_pStateMachine;}   
	Goal*const           HomeGoal()const	{return m_pHomeGoal;}
	Goal*const           OpponentsGoal()const{return m_pOpponentsGoal;}
	bool				 AllPlayersAtHome()const;
	SoccerPitch*const    Pitch()const		{return m_pPitch;}           
	SoccerTeam*const     Opponents()const	{return m_pOpponents;}

	team_color           Color()const{return m_Color;}

	void                 SetOpponents(SoccerTeam* opps){m_pOpponents = opps;}
	void                 SetPlayerClosestToBall(PlayerBase* plyr){m_pPlayerClosestToBall=plyr;}

	PlayerBase*          PlayerClosestToBall()const{return m_pPlayerClosestToBall;}
	double               ClosestDistToBallSq()const{return m_dDistSqToBallOfClosestPlayer;}

	Vector2D             GetSupportSpot()const{return m_pSupportSpotCalc->GetBestSupportingSpot();}

	PlayerBase*          SupportingPlayer()const{return m_pSupportingPlayer;}
	void                 SetSupportingPlayer(PlayerBase* plyr=NULL){m_pSupportingPlayer = plyr;}

	PlayerBase*          Receiver()const{return m_pReceivingPlayer;}

	bool				 IsReceiver() const { return m_pReceivingPlayer ==NULL; }
	void                 SetReceiver(PlayerBase* plyr){m_pReceivingPlayer = plyr;}

	PlayerBase*          ControllingPlayer()const{return m_pControllingPlayer;}
	void                 SetControllingPlayer(PlayerBase* plyr)
	{
		m_pControllingPlayer = plyr;
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

	std::string Name() const 
	{
		if (m_Color == blue) 
			return "Blue"; 
		return "Red";
	}

	bool  IsChaseBall() const	   { return m_IsChase; }
	void  SetChaseBall(bool chase) { m_IsChase = chase;}
	bool  IsThrowIn() const	       { return m_IsThrowIn; }
	void  SetThrowIn(bool throwin) { m_IsThrowIn = throwin;}
	Vector2D  HomeGoalFacing()	   { return m_pHomeGoal->Facing();}

private:
	///> 创建成员
	void CreatePlayers();

	///> 每一帧计算当前离球最近的球员
	void CalculateClosestPlayerToBall();			

	bool m_IsChase; 
	bool m_IsThrowIn;

private:

	StateMachine<SoccerTeam>*  m_pStateMachine;		 ///> 当前队伍的状态机
	team_color                m_Color;				 ///> 队伍颜色
	std::vector<PlayerBase*>  m_Players;			 ///> 队伍所有的成员
	SoccerPitch*              m_pPitch;				 ///> 指向的球场指针
	Goal*                     m_pOpponentsGoal;		 ///> 对方球门
	Goal*                     m_pHomeGoal;			 ///> 自己球门
	SoccerTeam*               m_pOpponents;			 ///> 敌对队伍指针
	PlayerBase*               m_pControllingPlayer;	 ///> 控球队员
	PlayerBase*               m_pSupportingPlayer;	 ///> 接应队员
	PlayerBase*               m_pReceivingPlayer;	 ///> 指向接球队员
	PlayerBase*               m_pPlayerClosestToBall;///> 指向离球最近的队员
	SupportSpotCalculator*    m_pSupportSpotCalc;	 ///>  用来决策队员的行为类

	double                    m_dDistSqToBallOfClosestPlayer;///> 离最近球员最近的距离


};