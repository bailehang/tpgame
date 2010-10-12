#include "Stdafx.h"
#include "SoccerPitch.h"
#include "SoccerTeam.h"
#include "Goal.h"
#include "Regulator.h"
#include "SupportSpotCalculator.h"
#include "SteeringBehaviors.h"
#include "Entity/PlayerBase.h"
#include "Entity/SoccerBall.h"
#include "Entity/GoalKeeper.h"
#include "Entity/FieldPlayer.h"
#include "Entity/EntityManager.h"
#include "StateAi/StateMachine.h"
#include "Messageing/MessageDispatcher.h"
#include "../Public/Singleton.h"
#include "../Render/VGdi.h"
#include "../Render/Vector2D.h"
#include "../Render/Geometry.h"

const int NumRegionsHorizontal = 12; 
const int NumRegionsVertical   = 4;

SoccerPitch::SoccerPitch(int cx, int cy):m_cxClient(cx),
	m_cyClient(cy),
	m_bPaused(false),
	m_bGoalKeeperHasBall(false),
	m_Regions(NumRegionsHorizontal*NumRegionsVertical),
	m_bGameOn(true)
{
	//define the playing area
	m_pPlayingArea = new Region(60, 30, cx-60, cy-30);

	//create the regions  
	CreateRegions(PlayingArea()->Width() / (double)NumRegionsHorizontal,
		PlayingArea()->Height() / (double)NumRegionsVertical);

	//create the goals
	m_pRedGoal  = new Goal(Vector2D( m_pPlayingArea->Left(), (cy-GetInstObj(CGameSetup).GoalWidth)/2),
		Vector2D(m_pPlayingArea->Left(), cy - (cy-GetInstObj(CGameSetup).GoalWidth)/2),
		Vector2D(1,0));


	m_pBlueGoal = new Goal( Vector2D( m_pPlayingArea->Right(), (cy-GetInstObj(CGameSetup).GoalWidth)/2),
		Vector2D(m_pPlayingArea->Right(), cy - (cy-GetInstObj(CGameSetup).GoalWidth)/2),
		Vector2D(-1,0));


	//create the soccer ball
	m_pBall = new SoccerBall(Vector2D((double)m_cxClient/2.0, (double)m_cyClient/2.0),
		GetInstObj(CGameSetup).BallSize,
		GetInstObj(CGameSetup).BallMass,
		m_vecWalls);


	//create the teams 
	m_pRedTeam  = new SoccerTeam(m_pRedGoal, m_pBlueGoal, this, SoccerTeam::red);
	m_pBlueTeam = new SoccerTeam(m_pBlueGoal, m_pRedGoal, this, SoccerTeam::blue);

	//make sure each team knows who their opponents are
	m_pRedTeam->SetOpponents(m_pBlueTeam);
	m_pBlueTeam->SetOpponents(m_pRedTeam); 

	//create the walls
	Vector2D TopLeft(m_pPlayingArea->Left(), m_pPlayingArea->Top());                                        
	Vector2D TopRight(m_pPlayingArea->Right(), m_pPlayingArea->Top());
	Vector2D BottomRight(m_pPlayingArea->Right(), m_pPlayingArea->Bottom());
	Vector2D BottomLeft(m_pPlayingArea->Left(), m_pPlayingArea->Bottom());

	m_vecWalls.push_back(Wall2D(BottomLeft, m_pRedGoal->RightPost()));
	m_vecWalls.push_back(Wall2D(m_pRedGoal->LeftPost(), TopLeft));
	m_vecWalls.push_back(Wall2D(TopLeft, TopRight));
	m_vecWalls.push_back(Wall2D(TopRight, m_pBlueGoal->LeftPost()));
	m_vecWalls.push_back(Wall2D(m_pBlueGoal->RightPost(), BottomRight));
	m_vecWalls.push_back(Wall2D(BottomRight, BottomLeft));

	//ParamLoader* p = ParamLoader::Instance();
}

SoccerPitch::~SoccerPitch()
{
	SAFE_DELETE( m_pBall );

	SAFE_DELETE( m_pRedTeam );
	SAFE_DELETE( m_pBlueTeam);

	SAFE_DELETE( m_pRedGoal );
	SAFE_DELETE( m_pBlueGoal);

	SAFE_DELETE( m_pPlayingArea );

	for (unsigned int i=0; i<m_Regions.size(); ++i)
	{
		SAFE_DELETE( m_Regions[i] );
	}
}

void SoccerPitch::Update()
{
	if (m_bPaused) return;

	static int tick = 0;

	//update the balls
	m_pBall->Update();

	//update the teams
	m_pRedTeam->Update();
	m_pBlueTeam->Update();

	//if a goal has been detected reset the pitch ready for kickoff
	if (m_pBlueGoal->Scored(m_pBall) || m_pRedGoal->Scored(m_pBall))
	{
		m_bGameOn = false;

		//reset the ball                                                      
		m_pBall->PlaceAtPosition(Vector2D((double)m_cxClient/2.0, (double)m_cyClient/2.0));

		//get the teams ready for kickoff
		m_pRedTeam->GetFSM()->ChangeState(&GetInstObj(PrepareForKickOff));
		m_pBlueTeam->GetFSM()->ChangeState(&GetInstObj(PrepareForKickOff));
	}
}

void SoccerPitch::CreateRegions(double width, double height)
{  
	int idx = m_Regions.size()-1;

	for (int col=0; col<NumRegionsHorizontal; ++col)
	{
		for (int row=0; row<NumRegionsVertical; ++row)
		{
			m_Regions[idx--] = new Region(PlayingArea()->Left()+col*width,
				PlayingArea()->Top()+row*height,
				PlayingArea()->Left()+(col+1)*width,
				PlayingArea()->Top()+(row+1)*height,
				idx);
		}
	}
}


bool SoccerPitch::Render()
{
	//render regions
	if (GetInstObj(CGameSetup).bRegions)
	{   
		for (unsigned int r=0; r<m_Regions.size(); ++r)
		{
			m_Regions[r]->Render(true);
		}
	}

	//GetInstObj(CGDI).HollowBrush();
	GetInstObj(CGDI).RedPen();
	GetInstObj(CGDI).HatchRBrush();
	GetInstObj(CGDI).Rect(m_pPlayingArea->Left()-40, (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2, m_pPlayingArea->Left(), m_cyClient - (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2);
	GetInstObj(CGDI).Rect(m_pPlayingArea->Left()-39, (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2+1, m_pPlayingArea->Left()-1, m_cyClient - (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2-1);	
	GetInstObj(CGDI).HatchXBrush();
	GetInstObj(CGDI).Rect(m_pPlayingArea->Left(), (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2-100, m_pPlayingArea->Left()+100, m_cyClient - (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2+100);
	GetInstObj(CGDI).Rect(m_pPlayingArea->Left()+1, (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2-99, m_pPlayingArea->Left()+99, m_cyClient - (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2+99);


	GetInstObj(CGDI).BluePen();
	GetInstObj(CGDI).HatchLBrush();
	GetInstObj(CGDI).Rect(m_pPlayingArea->Right(), (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2, m_pPlayingArea->Right()+40, m_cyClient - (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2);
	GetInstObj(CGDI).Rect(m_pPlayingArea->Right()+1, (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2+1, m_pPlayingArea->Right()+39, m_cyClient - (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2-1);

	GetInstObj(CGDI).HatchXBrush();
	GetInstObj(CGDI).Rect(m_pPlayingArea->Right()-100, (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2-100, m_pPlayingArea->Right(), m_cyClient - (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2+100);
	GetInstObj(CGDI).Rect(m_pPlayingArea->Right()-99, (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2-99, m_pPlayingArea->Right()-1, m_cyClient - (m_cyClient-GetInstObj(CGameSetup).GoalWidth)/2+99);

	//render the pitch markings
	GetInstObj(CGDI).ClearBrush();

	GetInstObj(CGDI).WhitePen();
	GetInstObj(CGDI).Circle(m_pPlayingArea->Center(), m_pPlayingArea->Width() * 0.125);
	GetInstObj(CGDI).Line(m_pPlayingArea->Center().x, m_pPlayingArea->Top(), m_pPlayingArea->Center().x, m_pPlayingArea->Bottom());
	GetInstObj(CGDI).WhiteBrush();
	GetInstObj(CGDI).Circle(m_pPlayingArea->Center(), 3.0);


	//the ball
	GetInstObj(CGDI).WhitePen();
	GetInstObj(CGDI).WhiteBrush();
	m_pBall->Render();

	//Render the teams
	m_pRedTeam->Render();
	m_pBlueTeam->Render(); 

	//render the walls
	GetInstObj(CGDI).WhitePen();
	for (unsigned int w=0; w<m_vecWalls.size(); ++w)
	{
		m_vecWalls[w].Render();
	}

	//show the score
	GetInstObj(CGDI).TextColor(CGDI::red);
	GetInstObj(CGDI).TextAtPos((m_cxClient/2)-80, 10, "红队得分: " + ttos(m_pBlueGoal->NumGoalsScored()));

	GetInstObj(CGDI).TextColor(CGDI::blue);
	GetInstObj(CGDI).TextAtPos((m_cxClient/2)+50, 10, "蓝队得分: " + ttos(m_pRedGoal->NumGoalsScored()));

	return true;  
}