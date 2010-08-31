
#pragma  once 

#include "MoveEntity.h"
#include "../Wall2D.h"
#include "../../Public/GameSetup.h"
#include "../../Public/Singleton.h"
#include "../../Render/VGdi.h"

///	 ◊„«Ú¿‡
class  SoccerBall : public CMoveEntity
{
public:
	SoccerBall(Vector2D      pos,            
			   double        BallSize,
			   double        mass,
		std::vector<Wall2D>& PitchBoundary):m_PitchBoundary(PitchBoundary),
		CMoveEntity(pos,
			BallSize,
			Vector2D(0,0),
			-1.0,                
			Vector2D(0,1),
			mass,
			Vector2D(1.0,1.0),  
			0,                 
			0)		
	{

	}

	~SoccerBall(){}

public:

	void TestCollisionWithWalls(const std::vector<Wall2D>& walls);

	//implement base class Update
	void      Update();

	//implement base class Render
	void      Render();

	//a soccer ball doesn't need to handle messages
	bool      HandleMessage(const Telegram& msg){return false;}

	//this method applies a directional force to the ball (kicks it!)
	void      Kick(Vector2D direction, double force);

	//given a kicking force and a distance to traverse defined by start
	//and finish points, this method calculates how long it will take the
	//ball to cover the distance.
	double    TimeToCoverDistance(Vector2D from,
		Vector2D to,
		double     force)const;

	//this method calculates where the ball will in 'time' seconds
	Vector2D FuturePosition(double time)const;

	void      Trap(){m_vVelocity.Zero();}  

	Vector2D  OldPos()const{return m_vOldPos;}

	//this places the ball at the desired location and sets its velocity to zero
	void      PlaceAtPosition(Vector2D NewPos);

private:

	//keeps a record of the ball's position at the last update
	Vector2D                  m_vOldPos;

	//a local reference to the Walls that make up the pitch boundary
	const std::vector<Wall2D>& m_PitchBoundary;   

};