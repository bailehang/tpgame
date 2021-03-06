

#pragma  once

#include "../../Public/Common/Autolist.h"
#include "../../Public/MsgImpl.h"
#include "../Region.h"
#include "../FootBallTeam.h"
#include "MoveEntity.h"

class FootBall;
class FootBall;
class SteeringBehaviors;

/// 球员基类类 
class  BasePlayer : public  CMoveEntity
{

public:
	enum player_role{goal_keeper, attacker, defender};
public:
	BasePlayer(FootBallTeam*    home_team,
				int            home_region,
				Vector2D       heading,
				Vector2D       velocity,
				double          mass,
				double          max_force,
				double          max_speed,
				double          max_turn_rate,
				double          scale,
				player_role    role);

	virtual ~BasePlayer();

	/// 玩家监控范围内是否有对方球员
	bool        isThreatened()const;

	/// 追球和追踪目标 
	void        TrackBall();
	void        TrackTarget();

	/// 开始组织队伍，让离自身最近的玩家改变为接应状态，方便进行进攻
	void        FindSupport()const;

	/// 判断球是否能被守门员捕获
	bool        BallWithinKeeperRange()const;

	/// 判断当前对象是否能够踢到球，
	bool        BallWithinKickingRange()const;

	/// 判断当前对象是否能够接收到球
	bool        BallWithinReceivingRange()const;

	/// 是否已经回到出身点
	bool        InHomeRegion()const;

	/// 是否在当前进攻带球球员的前面
	bool        isAheadOfAttacker()const;

	/// 是否到达接应点的范围
	bool        AtTarget()const;

	/// 跟踪目标
	bool		FollowTarget()const;

	/// 返回
	bool		FollowReturn()const;

	/// 是否已经在出生位置区域
	bool        IsSelfRegin()const;

	/// 是否是队伍离球最近的队员
	bool        isClosestTeamMemberToBall()const;

	///	 查看position是否在队员前面
	bool        PositionInFrontOfPlayer(Vector2D position)const;

	/// 是否是离球最近的球员
	bool        isClosestPlayerOnPitchToBall()const;

	/// 队伍是否是控制足球
	bool        isControllingPlayer()const;

	/// 是否在禁区
	bool        InHotRegion()const;

	player_role Role()const{return m_PlayerRole;}

	double      DistSqToBall()const{return m_dDistSqToBall;}
	void        SetDistSqToBall(double val){m_dDistSqToBall = val;}

	/// 离对方和自身球门的距离
	double       DistToOppGoal()const;
	double       DistToHomeGoal()const;

	void         SetDefaultHomeRegion(){m_iHomeRegion = m_iDefaultRegion;}

	FootBall*  const		 Ball()const;
	Vector2D				 BallPos() const;
	void					 SetSteeringTarget( Vector2D& vec );
	FootBallPitch* const     Pitch()const;
	SteeringBehaviors*const  Steering()const{return m_pSteering;}
	const Region* const      HomeRegion()const;
	bool					 InsideHomeRegion(Vector2D pos, int r)const;
	void                     SetHomeRegion(int NewRegion){m_iHomeRegion = NewRegion;}
	FootBallTeam*const       Team()const{return m_pTeam;}

	bool					 IsChaseBall()const { return m_pTeam->IsChaseBall();}
	Vector2D				 GetHomeCenter()    { return HomeRegion()->Center();}

protected:
	player_role				m_PlayerRole;	 ///> 当前队员角色
	FootBallTeam*             m_pTeam;		 ///> 当前队伍指针
	SteeringBehaviors*      m_pSteering;	 ///> 操作行为指针
	int                     m_iHomeRegion;	 ///> 队员出身区域
	int                     m_iDefaultRegion;///> 开球前的区域 
	double                  m_dDistSqToBall; ///> 离球的距离
	std::vector<Vector2D>   m_vecPlayerVB;	 ///> 球员的外观矩形
	std::vector<Vector2D>   m_vecPlayerVBTrans;///> 转换后的矩形

};