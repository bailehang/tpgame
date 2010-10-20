

#pragma once

#pragma warning (disable:4786)

#include <vector>
#include <windows.h>
#include <string>

#include "../Render/Vector2D.h"

class BasePlayer;
class FootBallPitch;
class FootBall;
class CWall;
class CObstacle;

class SteeringBehaviors
{
   	enum behavior_type
	{
		none               = 0x0000,
		seek               = 0x0001,
		arrive             = 0x0002,
		separation         = 0x0004,
		pursuit            = 0x0008,
		interpose          = 0x0010
	};
	enum Deceleration{slow = 3, normal = 2, fast = 1};
public:

	SteeringBehaviors(BasePlayer*       agent,
		FootBallPitch*  world,
		FootBall*   ball);

	virtual ~SteeringBehaviors(){}


	Vector2D Calculate();

	double    ForwardComponent();

	double    SideComponent();

	Vector2D Force()const{return m_vSteeringForce;}

	void      RenderInfo();
	void      RenderAids();

	Vector2D  Target()const{return m_vTarget;}
	void      SetTarget(const Vector2D t){m_vTarget = t;}
	double    InterposeDistance()const{return m_dInterposeDist;}
	void      SetInterposeDistance(double d){m_dInterposeDist = d;}
	bool      Tagged()const{return m_bTagged;}
	void      Tag(){m_bTagged = true;}
	void      UnTag(){m_bTagged = false;}


	void      SeekOn()   {m_iFlags |= seek;}
	void	  ArriveOn() {m_iFlags |= arrive;}
	void	  PursuitOn(){m_iFlags |= pursuit;}
	void	  SeparationOn(){m_iFlags |= separation;}
	void	  InterposeOn(double d){m_iFlags |= interpose; m_dInterposeDist = d;}
	void	  SeekOff()   {if(On(seek))   m_iFlags ^=seek;}
	void	  ArriveOff() {if(On(arrive)) m_iFlags ^=arrive;}
	void	  PursuitOff(){if(On(pursuit)) m_iFlags ^=pursuit;}
	void	  SeparationOff(){if(On(separation)) m_iFlags ^=separation;}
	void	  InterposeOff() {if(On(interpose)) m_iFlags ^=interpose;}


	bool SeekIsOn(){return On(seek);}
	bool ArriveIsOn(){return On(arrive);}
	bool PursuitIsOn(){return On(pursuit);}
	bool SeparationIsOn(){return On(separation);}
	bool InterposeIsOn(){return On(interpose);}

private:
	
	Vector2D Seek(Vector2D target);
	Vector2D Arrive(Vector2D target, Deceleration decel);
	Vector2D Pursuit(const FootBall* ball);
	Vector2D Separation();
	Vector2D Interpose(const FootBall* ball,
						Vector2D pos,
						double    DistFromTarget);
	void      FindNeighbours();
	bool      On(behavior_type bt){return (m_iFlags & bt) == bt;}
	bool      AccumulateForce(Vector2D &sf, Vector2D ForceToAdd);
	Vector2D  SumForces();

private:

	BasePlayer*		m_pPlayer;                                                  
	FootBall*		m_pBall;
	Vector2D		m_vSteeringForce;
	Vector2D		m_vTarget;
	double			m_dInterposeDist;
	double			m_dMultSeparation;
	double			m_dViewDistance;
	int				m_iFlags;
	bool			m_bTagged;
	std::vector<Vector2D> m_Antenna;

};
