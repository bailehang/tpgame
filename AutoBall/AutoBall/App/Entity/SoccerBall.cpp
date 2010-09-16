#include "Stdafx.h"
#include "SoccerBall.h"
#include "PlayerBase.h"
#include "../SoccerTeam.h"
#include "../SoccerPitch.h"
#include "../StateAi/StateMachine.h"
#include "../StateAi/State.h"
#include "../StateAi/TeamStates.h"


extern SoccerPitch* g_SoccerPitch;
//g_SoccerPitch->m_pBlueTeam
//-------------------------- Kick ----------------------------------------
//                                                                        
//  applys a force to the ball in the direction of heading. Truncates
//  the new velocity to make sure it doesn't exceed the max allowable.
//------------------------------------------------------------------------
void SoccerBall::Kick(Vector2D direction, double force,CMoveEntity* entity)
{  
	//ensure direction is normalized
	direction.Normalize();

	//calculate the acceleration
	Vector2D acceleration = (direction * force) / m_dMass;

	//update the velocity
	m_vVelocity = acceleration;
	m_LastPlayer= entity;

}

//----------------------------- Update -----------------------------------
//
//  updates the ball physics, tests for any collisions and adjusts
//  the ball's velocity accordingly
//------------------------------------------------------------------------
void SoccerBall::Update()
{
	//keep a record of the old position so the goal::scored method
	//can utilize it for goal testing
	m_vOldPos = m_vPosition;

	//Test for collisions
	TestCollisionWithWalls(m_PitchBoundary);

	//Simulate GetInstObj(CGameSetup).Friction. Make sure the speed is positive 
	//first though
	if (m_vVelocity.LengthSq() > GetInstObj(CGameSetup).Friction * GetInstObj(CGameSetup).Friction)
	{
		m_vVelocity += Vec2DNormalize(m_vVelocity) * GetInstObj(CGameSetup).Friction;

		m_vPosition += m_vVelocity;

		//update heading
		m_vHeading = Vec2DNormalize(m_vVelocity);
	}
	char  str[256];
	sprintf_s(str,"Ball Position x=%f,y=%f",m_vPosition.x,m_vPosition.y);
	PutLog("ball.txt",str);
}

//---------------------- TimeToCoverDistance -----------------------------
//
//  Given a force and a distance to cover given by two vectors, this
//  method calculates how long it will take the ball to travel between
//  the two points
//------------------------------------------------------------------------
double SoccerBall::TimeToCoverDistance(Vector2D A,
									   Vector2D B,
									   double force)const
{
	//this will be the velocity of the ball in the next time step *if*
	//the player was to make the pass. 
	double speed = force / m_dMass;

	//calculate the velocity at B using the equation
	//
	//  v^2 = u^2 + 2as
	//

	//first calculate s (the distance between the two positions)
	double DistanceToCover =  Vec2DDistance(A, B);

	double term = speed*speed + 2.0*DistanceToCover*GetInstObj(CGameSetup).Friction;

	//if  (u^2 + 2as) is negative it means the ball cannot reach point B.
	if (term <= 0.0) return -1.0;

	double v = sqrt(term);

	//it IS possible for the ball to reach B and we know its speed when it
	//gets there, so now it's easy to calculate the time using the equation
	//
	//    t = v-u
	//        ---
	//         a
	//
	return (v-speed)/GetInstObj(CGameSetup).Friction;
}

//--------------------- FuturePosition -----------------------------------
//
//  given a time this method returns the ball position at that time in the
//  future
//------------------------------------------------------------------------
Vector2D SoccerBall::FuturePosition(double time)const
{
	//using the equation s = ut + 1/2at^2, where s = distance, a = friction
	//u=start velocity

	//calculate the ut term, which is a vector
	Vector2D ut = m_vVelocity * time;

	//calculate the 1/2at^2 term, which is scalar
	double half_a_t_squared = 0.5 * GetInstObj(CGameSetup).Friction * time * time;

	//turn the scalar quantity into a vector by multiplying the value with
	//the normalized velocity vector (because that gives the direction)
	Vector2D ScalarToVector = half_a_t_squared * Vec2DNormalize(m_vVelocity);

	//the predicted position is the balls position plus these two terms
	return Pos() + ut + ScalarToVector;
}


//----------------------------- Render -----------------------------------
//
//  Renders the ball
//------------------------------------------------------------------------
void SoccerBall::Render()
{
	GetInstObj(CGDI).GreyBrush();

	GetInstObj(CGDI).Circle(m_vPosition, m_dBoundingRadius);

	/*
	GetInstObj(CGDI).GreenBrush();
	for (int i=0; i<IPPoints.size(); ++i)
	{
	GetInstObj(CGDI).Circle(IPPoints[i], 3);
	}
	*/
}


//----------------------- TestCollisionWithWalls -------------------------
//
void SoccerBall::TestCollisionWithWalls(const std::vector<Wall2D>& walls)
{  
	//test ball against each wall, find out which is closest
	int idxClosest = -1;

	Vector2D VelNormal = Vec2DNormalize(m_vVelocity);

	Vector2D IntersectionPoint, CollisionPoint;

	double DistToIntersection = MaxFloat;

	/// µÈ´ý±ß½çÇò
	if( m_vVelocity.Length() <= 0 )
		return ;

	//iterate through each wall and calculate if the ball intersects.
	//If it does then store the index into the closest intersecting wall
	for (unsigned int w=0; w<walls.size(); ++w)
	{
		//assuming a collision if the ball continued on its current heading 
		//calculate the point on the ball that would hit the wall. This is 
		//simply the wall's normal(inversed) multiplied by the ball's radius
		//and added to the balls center (its position)
		Vector2D ThisCollisionPoint = Pos() - (walls[w].Normal() * BRadius());

		//calculate exactly where the collision point will hit the plane    
		if (WhereIsPoint(ThisCollisionPoint,
			walls[w].From(),
			walls[w].Normal()) == plane_backside)
		{
			double DistToWall = DistanceToRayPlaneIntersection(ThisCollisionPoint,
				walls[w].Normal(),
				walls[w].From(),
				walls[w].Normal());

			IntersectionPoint = ThisCollisionPoint + (DistToWall * walls[w].Normal());

		}

		else
		{
			double DistToWall = DistanceToRayPlaneIntersection(ThisCollisionPoint,
				VelNormal,
				walls[w].From(),
				walls[w].Normal());

			IntersectionPoint = ThisCollisionPoint + (DistToWall * VelNormal);
		}

		//check to make sure the intersection point is actually on the line
		//segment
		bool OnLineSegment = false;

		if (LineIntersection2D(walls[w].From(), 
			walls[w].To(),
			ThisCollisionPoint - walls[w].Normal()*20.0,
			ThisCollisionPoint + walls[w].Normal()*20.0))
		{

			OnLineSegment = true;                                               
		}


		double distSq = Vec2DDistanceSq(ThisCollisionPoint, IntersectionPoint);

		if ((distSq <= m_vVelocity.LengthSq()) && (distSq < DistToIntersection) && OnLineSegment)            
		{        
			DistToIntersection = distSq;
			idxClosest = w;
			CollisionPoint = IntersectionPoint;
		}     
	}//next wall

	if ( (idxClosest >= 0 ) && VelNormal.Dot(walls[idxClosest].Normal()) < 0)
	{
		m_vVelocity = Vector2D(0,0);
		PlayerBase* Player = (PlayerBase*)m_LastPlayer;

		if( Player->Team()->Color() == SoccerTeam::blue )
		{
			g_SoccerPitch->m_pBlueTeam->GetFSM()->ChangeState( Throw_In::Instance() );
			g_SoccerPitch->m_pRedTeam->SetChaseBall(false);
			
		}
		else
		{
			g_SoccerPitch->m_pRedTeam->GetFSM()->ChangeState( Throw_In::Instance() );
			g_SoccerPitch->m_pBlueTeam->SetChaseBall(false);

		}

		//m_vVelocity.Reflect(walls[idxClosest].Normal());   
	}
}


void SoccerBall::PlaceAtPosition(Vector2D NewPos)
{
	m_vPosition = NewPos;

	m_vOldPos = m_vPosition;

	m_vVelocity.Zero();
}