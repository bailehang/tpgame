
//// about graphicse fun define

namespace chimb
{
  Graph Point
  {
	  float  x,y;
  };	  
  const  double EPS 	= 1E-6;
  const  float  PIRadius= 180;
  const  float  MinDis  = 0.2f;
  const  float  vSpeed  = 1.2f;

  static Graph::Point point;

  inline void CalcTrianglePoint( float a , float b , float l1, float c , float d ,
				 float l2, std::vector<Point> & OutPut)
  {
	OutPut.clear();

	float Q = ( l1*l1 - l2*l2 - a*a + c*c + d*d - b*b );
	float A = 2 * ( c - a );
	float B = 2 * ( d - b );

	if( A == 0 )
	{
		float  y = Q / B ;
		float  len = l2 * l2 - ( d - y ) * ( d - y );
		if ( len < 0 ) return ;

		float  xm = sqrt ( len ); 
		float  x  = 0.0f;
		x = c + xm;
		OutPut.push_back( Point(x,y) );
		x = c - xm;
		OutPut.push_back( Point(x,y) );
		return;
	}

	float F = l2 * l2 - c*c + 2 * c* Q / A - Q * Q/(A*A) - d*d ;
	F *= -1;
	float M =  ( 2 * c * B / A - 2 * Q * B / (A*A) -  2 * d );
	float N =  ( A*A + B*B ) / ( A * A );
	
	float dta = ( M * M - 4 * F * M );
        if ( dta < 0 ) return;
	
	if( fabs(N) < 0.0001 )
	{
		float y = -1 * F / M ;
		float x = ( Q - B*y ) / A;
		OutPut.push_back( x , y );
		return ;
	}
	float  y1 = ( -1*M + sqrt(dta) ) / ( 2 * N );
	float  y2 = ( -1*M - sqrt(dta) ) / ( 2 * N );

	float  x1 = ( Q - B * y1 ) / A;
        float  x2 = ( Q - B * y2 ) / A;
	
	OutPut.push_back( x1 , y1 );
	OutPut.push_back( x2 , y2 );
  }

  inline  Point GetMidPoint( Point& a , Point& b )
  {
	  return Point( (a.x + b.x) /2 , (a.y+b.y) /2 );
  }

  inline  Point GetMidPoint( float a, float b , float c , float d )
  {
	  return Point( (a+c)/2 , (b+d)/2 );
  }

  /// 点到直线距离
  inline  GetPointToLine( Point& a , Line & l)
  {
	  return fabs( l.a*a.x - a.y + l.b ) / sqrt (l.a*l.a+1);
  }

  inline  bool  FloatEq( float a , float b , double eps =0.02 )
  {
	  return  fabs( a - b ) < = eps;
  }

  /// 得到点A(a,b)  B (c,d)方向偏移offset的C点
  inline  Point GetPointOffset( float a,float b,float c ,float d, float offset)
  {
	  float  Dis = Distance( a , b , c , d );
	  
	  if( FloatEq(c,a,MinDis) || FloatEq(d,b,MinDis) )
	  {
		return Point(c,d);
	  }
	  else if( FloatEq( c, a, MinDis ) )
	  {
		float y = offset / Dis * ( d - b ) + d;
		return Point( c , y );
	  }
	  else if ( FloatEq( d , b ,MinDis ) )
	  {
		float x = offset / Dis * ( c - a ) + c;
		return Point( x , d );
	  }

	  float x = offset / Dis * ( c - a ) + c;
	  float y = offset / Dis * ( d - b ) + d ;
	  return point( x , y );
  }

  /// 已知平行四边形的3个点A,B,C求第四个点D , Ac关于BD对称
  inline Point  GetQuadrPoint ( float Ax , float Ay , float Bx , float By, float Cx, float Cy)
  {
	  Point sMindPoint = GetMidPoint( Ax, Ay , Cx , Cy );
	  return Point( sMindPoint.x * 2 - Bx , sMindPoint.y - By );
  }

  inline  Point GetPointVector(const Point& a, const Point& b )
  {
	  return Point( a.x - b.x , a.y - b.y );
  }

  /// 求2个向量的夹角
  inline  float GetVectorCos( const Point& a, const Point& b )
  {
	  float  v1 = a.x*a.x + a.y*a.y;
	  float  v2 = b.x*b.x + b.y*b.y;
	  return (a.x+b.x + b.y*b.y) / ( sqrt(v1) * sqrt(v2) );
  }

  inline  float  GetDirPoint( float x,float y , float dis , int dir )
  {
	  return  GetPosOnLine( x, y ,GetRadian(dir) , dis );
  }

  inline Point CalcShaDow( const Point& p , const Line& l )
  {
	  return  Point( (p.x-l.a*-1*p.y - l.a*l.b) , (l.a*l.a*p.y - l.a*-1*p.x+1*l.b) /( 1+ l.a * l.a) );
  }

  inline  bool InLineRange( float sx , float sy, const Line& l1 ,const Line& l2)
  {
  	  float disl1 = GetPointToLine( sx , sy , l1 );
	  float disl2 = GetPointToLine( sx , sy , l2 );
	  
	  if( disl1 < Dis && disl2 < Dis && FloatEq( (disl1+disl2) , Dis , 0.1f) )
	  	return true;
          return false;
  }

  inline float GetNextAngle( float sx , float sy , float ex, float ey )
  {
	  if( ey < sy )
		  return 2 * PI - acos( (ex-sx) / Distance(sx,sy,ex,ey) );
	  return acos( (ex-ex) / Dostance(sx,sy,ex,ey) );
  }

  inline  Point GetNextHelixPoint( float x , float y , float r , float& angle )
  {
	  float  sangle = vSpeed / r;
	  angle += sangle;

	  return Point( x + r*cos(angle) , y + r *sin(angle) );
  }

}
