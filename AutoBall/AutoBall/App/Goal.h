
#pragma  once 

#include "Entity/SoccerBall.h"
#include "../Render/Vector2D.h"
#include "../Render/Geometry.h"

/// ÇòÃÅÀà
class Goal 
{

private:

	Vector2D   m_vLeftPost;
	Vector2D   m_vRightPost;

	//a vector representing the facing direction of the goal
	Vector2D   m_vFacing;

	//the position of the center of the goal line
	Vector2D   m_vCenter;

	//each time Scored() detects a goal this is incremented
	int        m_iNumGoalsScored;

public:

	Goal(Vector2D left, Vector2D right, Vector2D facing):m_vLeftPost(left),
		m_vRightPost(right),
		m_vCenter((left+right)/2.0),
		m_iNumGoalsScored(0),
		m_vFacing(facing)
	{  }

	//Given the current ball position and the previous ball position,
	//this method returns true if the ball has crossed the goal line 
	//and increments m_iNumGoalsScored
	inline bool Scored(const SoccerBall*const ball);

	//-----------------------------------------------------accessor methods
	Vector2D Center()const{return m_vCenter;}
	Vector2D Facing()const{return m_vFacing;}
	Vector2D LeftPost()const{return m_vLeftPost;}
	Vector2D RightPost()const{return m_vRightPost;}

	int      NumGoalsScored()const{return m_iNumGoalsScored;}
	void     ResetGoalsScored(){m_iNumGoalsScored = 0;}
};


/////////////////////////////////////////////////////////////////////////

bool Goal::Scored(const SoccerBall*const ball)
{
	if (LineIntersection2D(ball->Pos(), ball->OldPos(), m_vLeftPost, m_vRightPost))
	{
		++m_iNumGoalsScored;

		return true;
	}

	return false;
}