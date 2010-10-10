
#pragma  once 

#include "Entity/SoccerBall.h"
#include "../Render/Vector2D.h"
#include "../Render/Geometry.h"

/// 球门类
class Goal 
{

public:

	Goal(Vector2D left, Vector2D right, Vector2D facing):m_vLeftPost(left),
		m_vRightPost(right),
		m_vCenter((left+right)/2.0),
		m_iNumGoalsScored(0),
		m_vFacing(facing)
	{  }

	/// 如果跨过球门口，改方法返回真
	inline bool Scored(const SoccerBall*const ball);

	Vector2D Center()const		{return m_vCenter;}
	Vector2D Facing()const		{return m_vFacing;}
	Vector2D LeftPost()const	{return m_vLeftPost;}
	Vector2D RightPost()const	{return m_vRightPost;}

	int    NumGoalsScored()const{return m_iNumGoalsScored;}
	void   ResetGoalsScored()	{m_iNumGoalsScored = 0;}

private:

	Vector2D   m_vLeftPost;				   ///> 左右2根门柱
	Vector2D   m_vRightPost;
	Vector2D   m_vFacing;				   ///> 球门的朝向向量
	Vector2D   m_vCenter;				   ///> 球门线的中间位置

	int        m_iNumGoalsScored;		  ///> 进球数量

};


/// 如果跨过球门口，改方法返回真
bool Goal::Scored(const SoccerBall*const ball)
{
	if (LineIntersection2D(ball->Pos(), ball->OldPos(), m_vLeftPost, m_vRightPost))
	{
		++m_iNumGoalsScored;

		return true;
	}

	return false;
}