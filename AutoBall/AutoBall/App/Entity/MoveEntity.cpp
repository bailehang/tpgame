#include "stdafx.h"
#include "MoveEntity.h"

void CMoveEntity::SetHeading(Vector2D new_heading)
{
	assert( (new_heading.LengthSq() - 1.0) < 0.00001);

	if( GetID() == 2 )
	{
		  char  str[256];
		  sprintf_s(str,"Position x=%f,y=%f",Heading().x,Heading().y);
		  char  p;
		  p='1';
	}

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

	//int vec = m_vHeading.Sign(toTarget);
	RotationMatrix.Rotate(angle * m_vHeading.Sign(toTarget));	
	if( GetID() == 2 )
	{
		  char  str[256];
		  sprintf_s(str,"Position x=%f,y=%f",Heading().x,Heading().y);
		  char  p;
		  p='1';
	}
	RotationMatrix.TransformVector2Ds(m_vHeading);
	RotationMatrix.TransformVector2Ds(m_vVelocity);

	m_vSide = m_vHeading.Perp();

	return false;
}
