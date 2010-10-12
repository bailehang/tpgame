#include "stdafx.h"
#include "Vector2D.h"

void  Vector2D::Zero()
{
	x = y = 0;
}

bool  Vector2D::IsZero() const
{
	return ( x*x + y*y <= tol);
}

double Vector2D::Length() const 
{
	return  sqrt( LengthSq() );
}

double Vector2D::LengthSq() const
{
	return  x*x + y*y;
}

void   Vector2D::Normalize()
{
	double nor = Length();

	if ( nor > std::numeric_limits<double>::epsilon())
	{
		this->x /= nor;
		this->y /= nor;
	}

	return;
}

double  Vector2D::Dot(const Vector2D& v2)const
{
	return  x*v2.x + y *v2.y;
}


int    Vector2D::Sign(const Vector2D &v2) const
{
	if ( y*v2.x > x*v2.y)
		return -1;
	else
		return 1;
}

Vector2D Vector2D::Perp() const
{
	return Vector2D( -y , x );
}

void  Vector2D::Truncate(double max)
{
	if ( Length() > max )
	{
		Normalize();

		*this  *=max ;
	}
}

double Vector2D::Distance(const Vector2D &v2) const
{
	return sqrt( DistanceSq(v2) );
}

double Vector2D::DistanceSq(const Vector2D &v2) const
{
	double  a = x - v2.x;
	double  b = y - v2.y;

	return  a*a + b*b;
}

void   Vector2D::Reflect(const Vector2D& v2) 
{
	Vector2D  result = 2.0 * this->Dot(v2) * v2.GetReverse();
	*this +=2.0 * this->Dot(v2) * v2.GetReverse();
}

Vector2D  Vector2D::GetReverse() const
{
	return Vector2D( -x , -y );
}

Vector2D& Vector2D::operator += ( const Vector2D& rhs )
{
	x = x + rhs.x ;
	y = y + rhs.y ;
	return  *this;
}

bool Vector2D::operator == ( const Vector2D& rhs )
{
	return  ( this->x == rhs.x && this->y == rhs.y );
}

Vector2D& Vector2D::operator -= ( const Vector2D& rhs )
{
	x = x - rhs.x ;
	y = y - rhs.y ;
	return  *this;
}

Vector2D& Vector2D::operator /= ( double rhs )
{
	x = x / rhs ;
	y = y / rhs ;
	return  *this;
}

Vector2D& Vector2D::operator *= ( double rhs )
{
	x = x * rhs ;
	y = y * rhs ;
	return  *this;
}

bool Vector2D::operator != ( const Vector2D& rhs )
{
	return ! (*this==rhs);
}


Vector2D operator * (const Vector2D& a , double vsys)
{
	Vector2D result(a);
	result *= vsys;
	return result;
}

Vector2D operator * ( double vsys , const Vector2D& a )
{
	Vector2D result(a);
	result *= vsys;
	return result;
}

Vector2D operator / (const Vector2D& a , double vsys )
{
	Vector2D result(a);
	result /= vsys;
	return result;
}

Vector2D operator + (const Vector2D& a , const Vector2D& b )
{
	Vector2D result(a);
	result.x += b.x;
	result.y += b.y;

	return result;
}

Vector2D operator - (const Vector2D& a , const Vector2D& b )
{
	Vector2D result(a);
	result.x -= b.x;
	result.y -= b.y;

	return result;
}


