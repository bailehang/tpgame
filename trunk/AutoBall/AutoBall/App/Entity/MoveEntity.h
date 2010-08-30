
#pragma  once 

#include "BaseEntity.h"
#include "../../Render/Vector2D.h"
#include "../../Render/Geometry.h"

class  CMoveEntity : public CBaseEntity
{

protected:

	Vector2D    m_vVelocity;
	Vector2D    m_vHeading;
	Vector2D    m_vSide; 
	double      m_dMass;
	double      m_dMaxSpeed;
	double      m_dMaxForce;   
	double      m_dMaxTurnRate;

public:
	CMoveEntity(Vector2D position,
				double   radius,
				Vector2D velocity,
				double   max_speed,
				Vector2D heading,
				double   mass,
				Vector2D scale,
				double   turn_rate,
				double   max_force):
				CBaseEntity(CBaseEntity::GetNextValidId()),
				m_vHeading(heading),
				m_vVelocity(velocity),
				m_dMass(mass),
				m_dMaxSpeed(max_speed),
				m_dMaxForce(max_force),
				m_dMaxTurnRate(turn_rate),
				m_vSide(m_vHeading.Perp())
				
	{
		m_vPosition = position;
		m_vScal = scale;
		m_dBoundingRadius = radius;
	}

	virtual ~CMoveEntity() {}
	
	Vector2D  Velocity()const{return m_vVelocity;}
	void      SetVelocity(const Vector2D& NewVel){m_vVelocity = NewVel;}

	double    Mass()const{return m_dMass;}

	Vector2D  Side()const{return m_vSide;}

	double    MaxSpeed()const{return m_dMaxSpeed;}                       
	void      SetMaxSpeed(double new_speed){m_dMaxSpeed = new_speed;}

	double    MaxForce()const{return m_dMaxForce;}
	void      SetMaxForce(double mf){m_dMaxForce = mf;}

	bool      IsSpeedMaxedOut()const{return m_dMaxSpeed*m_dMaxSpeed >= m_vVelocity.LengthSq();}
	double    Speed()const{return m_vVelocity.Length();}
	double    SpeedSq()const{return m_vVelocity.LengthSq();}

	Vector2D  Heading()const{return m_vHeading;}
	void      SetHeading(Vector2D new_heading);
	bool      RotateHeadingToFacePosition(Vector2D target);

	double    MaxTurnRate()const{return m_dMaxTurnRate;}
	void      SetMaxTurnRate(double val){m_dMaxTurnRate = val;}

};

void CMoveEntity::SetHeading(Vector2D new_heading)
{
	assert( (new_heading.LengthSq() - 1.0) < 0.00001);

	m_vHeading = new_heading;

	//the side vector must always be perpendicular to the heading
	m_vSide = m_vHeading.Perp();
}

bool CMoveEntity::RotateHeadingToFacePosition(Vector2D target)
{
	Vector2D toTarget = Vec2DNormalize(target - m_vPosition);

	double dot = m_vHeading.Dot(toTarget);

	Clamp(dot, -1, 1);

	double angle = acos(dot);

	if (angle < 0.00001) return true;

	if (angle > m_dMaxTurnRate) angle = m_dMaxTurnRate;

	C2DMatrix RotationMatrix;

	RotationMatrix.Rotate(angle * m_vHeading.Sign(toTarget));	
	RotationMatrix.TransformVector2Ds(m_vHeading);
	RotationMatrix.TransformVector2Ds(m_vVelocity);

	m_vSide = m_vHeading.Perp();

	return false;
}
