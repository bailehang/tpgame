
#pragma  once 

#include <vector>
#include "../Render/Vector2D.h"
#include "SpotCalculator.h"
#include "Goal.h"

class BasePlayer;
class FootBaller;
class FootBallPitch;
class GoalKeeper;
class SpotCalculator;
template<class T>
class StateMachine;
class SpotCalculator;
	  
class FootBallTeam 
{
public:

	enum team_color {blue, red};		  /// 球队类型

public:

	FootBallTeam(Goal*  home_goal,
			   Goal*  opponents_goal,
			   FootBallPitch* pitch,
			   team_color   color);

	~FootBallTeam();

	void        Render()const;
	void        Update();

	/// 让所有的队员归位
	void        ReturnAllFootBallerToHome()const;

	///  计算是否能射门进球
	bool        CanShootGoal(Vector2D  BallPos,
						 double    power, 
						 Vector2D& ShotTarget = Vector2D())const;

	///
	/// 队员调用次方法，决定是否能够传球，如果可以，那么谁是最佳的传球对象
	/// 什么位置又是最佳的传球位置
	/// 
	bool        FindPass(const BasePlayer*const passer,
						 BasePlayer*&  receiver,
						 Vector2D&     PassTarget,
						 double        power,
						 double        MinPassingDistance)const;

	/// 
	int		    FindRecvPass(const BasePlayer*const passer,
							 Vector2D& PassTarget,
							 double power,
							 double MinPassingDistance);

	/// 给定一个传球者和一个接应者，这个方法测试接球者附近的若干不同位置，
	/// 判断是否可以安全的传球，如果可以传球，
	/// 那么该方法把最佳传球保存在参数PassTarget中
	bool        GetBestPassToReceiver(const BasePlayer* const passer,
									  const BasePlayer* const receiver,
									  Vector2D& PassTarget,
									  const double power)const;

	/// 计算FORM到target对方球员opp是否可以夺走足球
	bool        isPassSafeFromOpponent( Vector2D    from,
										Vector2D    target,
										const BasePlayer* const receiver,
										const BasePlayer* const opp,
										double       PassingForce)const;

	/// 计算FORM到target对方所有的球员是否可以夺走足球
	bool        isPassSafeFromAllOpponents(Vector2D from,
						Vector2D target,
						const BasePlayer* const receiver,
						double     PassingForce)const;

	/// 对方球员进行队员的防御范围
	bool        isOpponentWithinRadius(Vector2D pos, double rad);

	/// 请求控球队员传球
	void        RequestPass(FootBaller* requester)const;

	/// 计算最佳传球位置，并查找最合适的对方队员前往这个地点
	BasePlayer* CalcBestSupportingAttacker();

	/// 得到所有的队内成员
	const std::vector<BasePlayer*>& Members()const{return m_Players;}  
	StateMachine<FootBallTeam>* GetFSM()const {return m_pStateMachine;}   
	Goal*const           HomeGoal()const	{return m_pHomeGoal;}
	Goal*const           OpponentsGoal()const{return m_pOpponentsGoal;}
	bool				 AllPlayersAtHome()const;
	FootBallPitch*const  Pitch()const		{return m_pPitch;}           
	FootBallTeam*const   Opponents()const	{return m_pOpponents;}

	team_color           Color()const{return m_Color;}

	void                 SetOpponents(FootBallTeam* opps){m_pOpponents = opps;}
	void                 SetPlayerClosestToBall(BasePlayer* plyr){m_pPlayerClosestToBall=plyr;}

	BasePlayer*          PlayerClosestToBall()const{return m_pPlayerClosestToBall;}
	double               ClosestDistToBallSq()const{return m_dDistSqToBallOfClosestPlayer;}

	Vector2D             GetSupportSpot()const{return m_pSupportSpotCalc->GetBestSupportingSpot();}

	BasePlayer*          SupportingPlayer()const{return m_pSupportingPlayer;}
	void                 SetSupportingPlayer(BasePlayer* plyr=NULL){m_pSupportingPlayer = plyr;}
	void                 SetSupportingNULLPlayer()				   {m_pSupportingPlayer = NULL;}

	BasePlayer*          Receiver()const	{ return m_pReceivingPlayer;	   }
	bool				 IsReceiver() const { return m_pReceivingPlayer!=NULL; }

	void                 SetReceiver(BasePlayer* plyr){m_pReceivingPlayer = plyr;}
	void				 SetNullReceiver()			  {m_pReceivingPlayer = NULL;}

	BasePlayer*          ControllingPlayer()const{return m_pControllingPlayer;}
	void                 SetControllingPlayer(BasePlayer* plyr)
	{
		m_pControllingPlayer = plyr;
		Opponents()->LostControl();
	}

	bool  InControl()const{
		if(m_pControllingPlayer)
			return true; 
		else return false;
	}

	bool IsControl()const 
	{
		if(m_pControllingPlayer)
			return true; 
		else return false;
	}
	void  LostControl(){m_pControllingPlayer = NULL;}
	BasePlayer*  GetPlayerFromID(int id)const;


	void SetPlayerHomeRegion(int plyr, int region)const;
	void BestSupportingPosition()const
	{
		m_pSupportSpotCalc->BestSupportingPosition();
	}

	void UpdateWaitingPlayers()const;

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

	void  SetThrow(int value)      { SetThrowIn(false);}

	Vector2D  HomeGoalFacing()	   { return m_pHomeGoal->Facing();}

private:
	///> 创建成员
	void CreatePlayers();

	///> 每一帧计算当前离球最近的球员
	void CalculateClosestPlayerToBall();			

	bool m_IsChase; 
	bool m_IsThrowIn;

private:

	StateMachine<FootBallTeam>*  m_pStateMachine;		 ///> 当前队伍的状态机
	team_color					 m_Color;				 ///> 队伍颜色
	std::vector<BasePlayer*>	 m_Players;				 ///> 队伍所有的成员
	GoalKeeper*					 m_GoalKeeper;			 ///> 守门员
	FootBallPitch*				 m_pPitch;				 ///> 指向的球场指针
	Goal*						 m_pOpponentsGoal;		 ///> 对方球门
	Goal*						 m_pHomeGoal;			 ///> 自己球门
	FootBallTeam*				 m_pOpponents;			 ///> 敌对队伍指针
	BasePlayer*                  m_pControllingPlayer;	 ///> 控球队员
	BasePlayer*					 m_pSupportingPlayer;	 ///> 接应队员
	BasePlayer*					 m_pReceivingPlayer;	 ///> 指向接球队员
	BasePlayer*					 m_pPlayerClosestToBall; ///> 指向离球最近的队员
	SpotCalculator*       m_pSupportSpotCalc;	 ///>  用来决策队员的行为类

	double                    m_dDistSqToBallOfClosestPlayer;///> 离最近球员最近的距离


};