/**
 *   @brief 可移动的实体类
 *
 */
#pragma  once 

#include "../../Render/Vector2D.h"
#include "../../Render/Geometry.h"
#include "BaseEntity.h"

class  CMoveEntity : public CBaseEntity
{


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
	
	Vector2D  Velocity()const					 {return m_vVelocity;  }
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

protected:

	Vector2D    m_vVelocity;	   ///> 实体速度
	Vector2D    m_vHeading;		   ///> 指向前方的方向向量
	Vector2D    m_vSide;	       ///> 方向的垂直向量
	double      m_dMass;		   ///> 质量
	double      m_dMaxSpeed;	   ///> 最大速度
	double      m_dMaxForce;	   ///> 最大转向力
	double      m_dMaxTurnRate;    ///> 最大转向速度

};

