#pragma once

#include <vector>
#include <assert.h>

class  FootBallTeam;
class  Goal;
class  FootBall;
class  Region;
class  Wall2D;

/// 足球场
class FootBallPitch
{ 

public:

	FootBallPitch(int cxClient, int cyClient);

	~FootBallPitch();

	void  Update();

	bool  Render();

	void  TogglePause(){m_bPaused = !m_bPaused;}
	bool  Paused()const{return m_bPaused;}

	int   cxClient()const{return m_cxClient;}
	int   cyClient()const{return m_cyClient;}

	bool  GoalKeeperHasBall()const{return m_bGoalKeeperHasBall;}
	void  SetGoalKeeperHasBall(bool b){m_bGoalKeeperHasBall = b;}

	const Region*const         PlayingArea()const{return m_pPlayingArea;}
	const std::vector<Wall2D>& Walls(){return m_vecWalls;}                      
	FootBall*const			   Ball()const{return m_pBall;}

	const Region* const GetRegionFromIndex(int idx)                                
	{
		assert ( (idx >= 0) && (idx < (int)m_Regions.size()) );

		return m_Regions[idx];
	}

	bool  GameOn()const{return m_bGameOn;}
	void  SetGameOn(){m_bGameOn = true;}
	void  SetGameOff(){m_bGameOn = false;}

private:

	void CreateRegions(double width, double height);/// 创建一个场景区域

public:

	FootBall*			 m_pBall;		///> 足球类 
	FootBallTeam*        m_pRedTeam;	///> 红队
	FootBallTeam*        m_pBlueTeam;	///> 蓝队

	Goal*                m_pRedGoal;	///> 红队球门
	Goal*                m_pBlueGoal;	///> 蓝队球门

	std::vector<Wall2D>  m_vecWalls;	///> 边界墙的容器

	Region*              m_pPlayingArea;///> 定义赛场的尺寸
	std::vector<Region*> m_Regions;		///> 记录所有的场景信息
	bool                 m_bGoalKeeperHasBall;///> 守门员是否控制了球
	bool                 m_bGameOn;		///> 记录游戏是否进行中，或者球进了中断比赛

	bool                 m_bPaused;	    ///> 游戏暂停
	int                  m_cxClient;    ///> 客户端窗口的本地副本尺寸
	int				   	 m_cyClient;  

};