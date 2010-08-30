#pragma once

#include "Entity/SoccerBall.h"
#include "SoccerTeam.h"
#include "../Render/Vector2D.h"
#include "../Render/Utils.h"
#include "../Render/VGdi.h"

#include "../Public/Singleton.h"
#include "../Public/GameSetup.h"
#include "../Render/Utils.h"


class SoccerPitch
{ 
public:

	SoccerBall*          m_pBall;

	SoccerTeam*          m_pRedTeam;
	SoccerTeam*          m_pBlueTeam;

	Goal*                m_pRedGoal;
	Goal*                m_pBlueGoal;

	//container for the boundary walls
	std::vector<Wall2D>  m_vecWalls;

	//defines the dimensions of the playing area
	Region*              m_pPlayingArea;

	//the playing field is broken up into regions that the team
	//can make use of to implement strategies.
	std::vector<Region*> m_Regions;

	//true if a goal keeper has possession
	bool                 m_bGoalKeeperHasBall;

	//true if the game is in play. Set to false whenever the players
	//are getting ready for kickoff
	bool                 m_bGameOn;

	//set true to pause the motion
	bool                 m_bPaused;

	//local copy of client window dimensions
	int                  m_cxClient,
		m_cyClient;  

	//this instantiates the regions the players utilize to  position
	//themselves
	void CreateRegions(double width, double height);


public:

	SoccerPitch(int cxClient, int cyClient);

	~SoccerPitch();

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
	SoccerBall*const           Ball()const{return m_pBall;}

	const Region* const GetRegionFromIndex(int idx)                                
	{
		assert ( (idx >= 0) && (idx < (int)m_Regions.size()) );

		return m_Regions[idx];
	}

	bool  GameOn()const{return m_bGameOn;}
	void  SetGameOn(){m_bGameOn = true;}
	void  SetGameOff(){m_bGameOn = false;}

};