#ifndef GEOMETRY_H
#define GEOMETRY_H
//------------------------------------------------------------------------
//Name:   geometry.h 
//Desc:   useful 2D geometry functions
//------------------------------------------------------------------------
#include "utils.h"
#include "Vector2D.h"
#include "Matrix.h"
#include "TransFuns.h"
#include <math.h>
#include <vector>


inline double DistanceToRayPlaneIntersection(Vector2D RayOrigin,
											 Vector2D RayHeading,
											 Vector2D PlanePoint,  //any point on the plane
											 Vector2D PlaneNormal)
{

	double d     = - PlaneNormal.Dot(PlanePoint);
	double numer = PlaneNormal.Dot(RayOrigin) + d;
	double denom = PlaneNormal.Dot(RayHeading);

	// normal is parallel to vector
	if ((denom < 0.000001) && (denom > -0.000001))
	{
		return (-1.0);
	}

	return -(numer / denom);	
}

//------------------------- WhereIsPoint --------------------------------------
enum span_type{plane_backside, plane_front, on_plane};
inline span_type WhereIsPoint(Vector2D point,
							  Vector2D PointOnPlane, //any point on the plane
							  Vector2D PlaneNormal) 
{
	Vector2D dir = PointOnPlane - point;

	double d = dir.Dot(PlaneNormal);

	if (d<-0.000001)
	{
		return plane_front;	
	}

	else if (d>0.000001)
	{
		return plane_backside;	
	}

	return on_plane;	
}



//-------------------------- GetRayCircleIntersec -----------------------------
inline double GetRayCircleIntersect(Vector2D RayOrigin,
									Vector2D RayHeading,
									Vector2D CircleOrigin,
									double  radius)
{

	Vector2D ToCircle = CircleOrigin-RayOrigin;
	double length      = ToCircle.Length();
	double v           = ToCircle.Dot(RayHeading);
	double d           = radius*radius - (length*length - v*v);

	// If there was no intersection, return -1
	if (d < 0.0) return (-1.0);

	// Return the distance to the [first] intersecting point
	return (v - sqrt(d));
}

//----------------------------- DoRayCircleIntersect --------------------------
inline bool DoRayCircleIntersect(Vector2D RayOrigin,
								 Vector2D RayHeading,
								 Vector2D CircleOrigin,
								 double     radius)
{

	Vector2D ToCircle = CircleOrigin-RayOrigin;
	double length      = ToCircle.Length();
	double v           = ToCircle.Dot(RayHeading);
	double d           = radius*radius - (length*length - v*v);

	// If there was no intersection, return -1
	return (d < 0.0);
}

inline bool GetTangentPoints (Vector2D C, double R, Vector2D P, Vector2D& T1, Vector2D& T2)
{
	Vector2D PmC = P - C;
	double SqrLen = PmC.LengthSq();
	double RSqr = R*R;
	if ( SqrLen <= RSqr )
	{
		// P is inside or on the circle
		return false;
	}

	double InvSqrLen = 1/SqrLen;
	double Root = sqrt(fabs(SqrLen - RSqr));

	T1.x = C.x + R*(R*PmC.x - PmC.y*Root)*InvSqrLen;
	T1.y = C.y + R*(R*PmC.y + PmC.x*Root)*InvSqrLen;
	T2.x = C.x + R*(R*PmC.x + PmC.y*Root)*InvSqrLen;
	T2.y = C.y + R*(R*PmC.y - PmC.x*Root)*InvSqrLen;

	return true;
}

inline double DistToLineSegment(Vector2D A,
								Vector2D B,
								Vector2D P)
{
	//if the angle is obtuse between PA and AB is obtuse then the closest
	//vertex must be A
	double dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

	if (dotA <= 0) return Vec2DDistance(A, P);

	//if the angle is obtuse between PB and AB is obtuse then the closest
	//vertex must be B
	double dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);

	if (dotB <= 0) return Vec2DDistance(B, P);

	//calculate the point along AB that is the closest to P
	Vector2D Point = A + ((B - A) * dotA)/(dotA + dotB);

	//calculate the distance P-Point
	return Vec2DDistance(P,Point);
}

inline double DistToLineSegmentSq(Vector2D A,
								  Vector2D B,
								  Vector2D P)
{
	//if the angle is obtuse between PA and AB is obtuse then the closest
	//vertex must be A
	double dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

	if (dotA <= 0) return Vec2DDistanceSq(A, P);

	//if the angle is obtuse between PB and AB is obtuse then the closest
	//vertex must be B
	double dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);

	if (dotB <= 0) return Vec2DDistanceSq(B, P);

	//calculate the point along AB that is the closest to P
	Vector2D Point = A + ((B - A) * dotA)/(dotA + dotB);

	//calculate the distance P-Point
	return Vec2DDistanceSq(P,Point);
}

inline bool LineIntersection2D(Vector2D A,
							   Vector2D B,
							   Vector2D C, 
							   Vector2D D)
{
	double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
	double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);

	double Bot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

	if (Bot == 0)//parallel
	{
		return false;
	}

	double invBot = 1.0/Bot;
	double r = rTop * invBot;
	double s = sTop * invBot;

	if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
	{
		//lines intersect
		return true;
	}

	//lines do not intersect
	return false;
}

inline bool LineIntersection2D(Vector2D A,
							   Vector2D B,
							   Vector2D C, 
							   Vector2D D,
							   double &dist)
{

	double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
	double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);

	double Bot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);


	if (Bot == 0)//parallel
	{
		if (isEqual(rTop, 0) && isEqual(sTop, 0))
		{
			return true;
		}
		return false;
	}

	double r = rTop/Bot;
	double s = sTop/Bot;

	if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
	{
		dist = Vec2DDistance(A,B) * r;

		return true;
	}

	else
	{
		dist = 0;

		return false;
	}
}

inline bool LineIntersection2D(Vector2D   A,
							   Vector2D   B,
							   Vector2D   C, 
							   Vector2D   D,
							   double&     dist,
							   Vector2D&  point)
{

	double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
	double rBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

	double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
	double sBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

	if ( (rBot == 0) || (sBot == 0))
	{
		//lines are parallel
		return false;
	}

	double r = rTop/rBot;
	double s = sTop/sBot;

	if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
	{
		dist = Vec2DDistance(A,B) * r;

		point = A + r * (B - A);

		return true;
	}

	else
	{
		dist = 0;

		return false;
	}
}

inline bool ObjectIntersection2D(const std::vector<Vector2D>& object1,
								 const std::vector<Vector2D>& object2)
{
	//test each line segment of object1 against each segment of object2
	for (unsigned int r=0; r<object1.size()-1; ++r)
	{
		for (unsigned int t=0; t<object2.size()-1; ++t)
		{
			if (LineIntersection2D(object2[t],
				object2[t+1],
				object1[r],
				object1[r+1]))
			{
				return true;
			}
		}
	}

	return false;
}

inline bool TwoCirclesOverlapped(double x1, double y1, double r1,
								 double x2, double y2, double r2)
{
	double DistBetweenCenters = sqrt( (x1-x2) * (x1-x2) +
		(y1-y2) * (y1-y2));

	if ((DistBetweenCenters < (r1+r2)) || (DistBetweenCenters < fabs(r1-r2)))
	{
		return true;
	}

	return false;
}

inline bool TwoCirclesOverlapped(Vector2D c1, double r1,
								 Vector2D c2, double r2)
{
	double DistBetweenCenters = sqrt( (c1.x-c2.x) * (c1.x-c2.x) +
		(c1.y-c2.y) * (c1.y-c2.y));

	if ((DistBetweenCenters < (r1+r2)) || (DistBetweenCenters < fabs(r1-r2)))
	{
		return true;
	}

	return false;
}

inline bool TwoCirclesEnclosed(double x1, double y1, double r1,
							   double x2, double y2, double r2)
{
	double DistBetweenCenters = sqrt( (x1-x2) * (x1-x2) +
		(y1-y2) * (y1-y2));

	if (DistBetweenCenters < fabs(r1-r2))
	{
		return true;
	}

	return false;
}


inline double CircleArea(double radius)
{
	return pi * radius * radius;
}

inline bool PointInCircle(Vector2D Pos,
						  double    radius,
						  Vector2D p)
{
	double DistFromCenterSquared = (p-Pos).LengthSq();

	if (DistFromCenterSquared < (radius*radius))
	{
		return true;
	}

	return false;
}


#endif
