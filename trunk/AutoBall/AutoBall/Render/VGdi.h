/**
 * brief  about render of gid class 
 * file   vgdi.h
 * date   2010.06.20
 */

#pragma  once 

#include <string>
#include <assert.h>
#include <vector>
#include "Vector2D.h"

#define   SNumber   15

const  COLORREF  colors[ SNumber ] = 
{
	RGB(255,0,0),
	RGB(0,0,255),
	RGB(0,255,0),
	RGB(0,0,0),
	RGB(255,200,200),
	RGB(200,200,200),
	RGB(255,255,0),
	RGB(255,170,0),
	RGB(255,0,170),
	RGB(133,90,0),
	RGB(255,255,255),  
	RGB(0, 72, 0),          //dark green
	RGB(0, 255, 255),       //light blue
	RGB(200, 200, 200),     //light grey
	RGB(255, 230, 230)      //light pink
};


class   CGDI
{

public:
	int   NumPenColors() const  { return SNumber ; }

	enum
	{
		red,
		blue,
		green,
		black,
		pink,
		grey,
		yellow,
		orange,
		purple,
		brown,   
		white,
		dark_green,
		light_blue,
		light_grey,
		light_pink,
		hollow
	};

private:

	HPEN    m_OldPen;

	// all the pens 
	HPEN	m_redPen;
	HPEN	m_bluePen;
	HPEN    m_greenPen;
	HPEN    m_blackPen;
	HPEN    m_pinkPen;
	HPEN    m_greyPen;
	HPEN    m_yellowPen;
	HPEN    m_orangePen;
	HPEN    m_purplePen;
	HPEN    m_brownPen;
	HPEN    m_whitePen;

	HPEN    m_darkgreenPen;
	HPEN    m_lightbluePen;
	HPEN    m_lightgreyPen;
	HPEN    m_lightPinkPen;

	HPEN   m_thickblackPen;
	HPEN   m_thickwhitePen;
	HPEN   m_thickredPen;
	HPEN   m_thickgreenPen;
	HPEN   m_thickbluePen; 

	HBRUSH m_oldBrush;

	//all the brushes
	HBRUSH  m_redBrush;
	HBRUSH  m_greenBrush;
	HBRUSH  m_blueBrush;
	HBRUSH  m_greyBrush;
	HBRUSH  m_brownBrush;
	HBRUSH  m_yellowBrush;
	HBRUSH  m_orangeBrush;
	HBRUSH  m_lightblueBrush;
	HBRUSH  m_darkgreenBrush;
	HBRUSH  m_whiteBrush;

	HDC     m_hdc;
	//
	CGDI(const CGDI&);
	CGDI& operator=(const CGDI&);
public:
	CGDI()  ;
	~CGDI() ;
	//CGDI(const CGDI& );

	static  CGDI* Instance();

	/// pen
	void  BlackPen()	 { if(m_hdc) SelectObject( m_hdc , m_blackPen) ; }
	void  RedPen()		 { if(m_hdc) SelectObject( m_hdc , m_redPen)   ; }
	void  GreenPen()	 { if(m_hdc) SelectObject( m_hdc , m_greenPen) ; }
	void  BluePen()		 { if(m_hdc) SelectObject( m_hdc , m_bluePen)  ; }
	void  PinkPen()		 { if(m_hdc) SelectObject( m_hdc , m_pinkPen)  ; }
	void  GreyPen()		 { if(m_hdc) SelectObject( m_hdc , m_greyPen)  ; }
	void  YellowPen()	 { if(m_hdc) SelectObject( m_hdc , m_yellowPen); }
	void  OrangePen()	 { if(m_hdc) SelectObject( m_hdc , m_orangePen); }
	void  PurplePen()    { if(m_hdc) SelectObject( m_hdc , m_purplePen); }
	void  BrownPen()	 { if(m_hdc) SelectObject( m_hdc , m_brownPen) ; }
	void  WhitePen()	 { if(m_hdc) SelectObject( m_hdc , m_whitePen) ; }
	void  DarkGreenPen() { if(m_hdc) SelectObject( m_hdc , m_darkgreenPen); }
	void  LightBluePen() { if(m_hdc) SelectObject( m_hdc , m_lightbluePen); }
	void  LightGreyPen() { if(m_hdc) SelectObject( m_hdc , m_lightgreyPen); }
	void  LightPinkPen() { if(m_hdc) SelectObject( m_hdc , m_lightPinkPen); }
	void  TickBlackPen() { if(m_hdc) SelectObject( m_hdc , m_thickblackPen);}
	void  TickWhitePen() { if(m_hdc) SelectObject( m_hdc , m_thickwhitePen);}
	void  TickRedPen()   { if(m_hdc) SelectObject( m_hdc , m_thickredPen)  ;}
	void  TickGreenPen() { if(m_hdc) SelectObject( m_hdc , m_thickgreenPen);}
	void  TickBluePen()  { if(m_hdc) SelectObject( m_hdc , m_thickbluePen) ;}

	// hrush
	void  BlackBrush()   {if(m_hdc)SelectObject(m_hdc, GetStockObject(BLACK_BRUSH));}
	void  WhiteBrush()   {if(m_hdc)SelectObject(m_hdc, GetStockObject(WHITE_BRUSH));} 
	void  HollowBrush()  {if(m_hdc)SelectObject(m_hdc, GetStockObject(HOLLOW_BRUSH));}
	void  GreenBrush()   {if(m_hdc)SelectObject(m_hdc, m_greenBrush);}
	void  RedBrush()	 {if(m_hdc)SelectObject(m_hdc, m_redBrush);}
	void  BlueBrush()    {if(m_hdc)SelectObject(m_hdc, m_blueBrush);}
	void  GreyBrush()	 {if(m_hdc)SelectObject(m_hdc, m_greyBrush);}
	void  BrownBrush()	 {if(m_hdc)SelectObject(m_hdc, m_brownBrush);}
	void  YellowBrush()  {if(m_hdc)SelectObject(m_hdc, m_yellowBrush);}
	void  LightBlueBrush() {if(m_hdc)SelectObject(m_hdc, m_lightblueBrush);}
	void  DarkGreenBrush() {if(m_hdc)SelectObject(m_hdc, m_darkgreenBrush);}
	void  OrangeBrush()  {if(m_hdc)SelectObject(m_hdc, m_orangeBrush);}
	//void  WhiteBrush()   {if(m_hdc)SelectObject(m_hdc, GetStockObject(WHITE_BRUSH));} 


	///  all menthods 
public:
	void   StartDrawing(HDC hdc)
	{
		assert(m_hdc == NULL);

		m_hdc = hdc;
		m_OldPen = (HPEN)SelectObject( hdc, m_blackPen);

		SelectObject(hdc,m_OldPen);
		m_oldBrush=(HBRUSH)SelectObject(hdc,GetStockObject(BLACK_BRUSH));
		SelectObject(hdc,m_oldBrush);
	}

	void  StopDrawing(HDC hdc)
	{
		assert(hdc != NULL);

		SelectObject(hdc, m_OldPen);
		SelectObject(hdc, m_oldBrush);

		m_hdc = NULL;
	}

	void  TextAtPos(int x,int y,const std::string &s)
	{
		TextOut(m_hdc, x, y, s.c_str(), (int)s.size());
	}

	void TextAtPos(double x, double y, const std::string &s)
	{
		TextOut(m_hdc, (int)x, (int)y, s.c_str(), (int)s.size());
	}

	void TextAtPos(Vector2D pos, const std::string &s)
	{
		TextOut(m_hdc, (int)pos.x, (int)pos.y, s.c_str(), (int)s.size());
	}

	void TransparentText(){SetBkMode(m_hdc, TRANSPARENT);}

	void OpaqueText(){SetBkMode(m_hdc, OPAQUE);}

	void TextColor(int color){assert(color < SNumber); SetTextColor(m_hdc, colors[color]);}
	void TextColor(int r, int g, int b){SetTextColor(m_hdc, RGB(r,g,b));}


	void DrawDot(Vector2D pos, COLORREF color)
	{
		SetPixel(m_hdc, (int)pos.x, (int)pos.y, color);
	}

	void DrawDot(int x, int y, COLORREF color)
	{
		SetPixel(m_hdc, x, y, color);
	}

	//-------------------------Line Drawing

	void Line(Vector2D from, Vector2D to)
	{
		MoveToEx(m_hdc, (int)from.x, (int)from.y, NULL);
		LineTo(m_hdc, (int)to.x, (int)to.y);
	}

	void Line(int a, int b, int x, int y)
	{
		MoveToEx(m_hdc, a, b, NULL);
		LineTo(m_hdc, x, y);
	}

	void Line(double a, double b, double x, double y)
	{
		MoveToEx(m_hdc, (int)a, (int)b, NULL);
		LineTo(m_hdc, (int)x, (int)y);
	}



	void PolyLine(const std::vector<Vector2D>& points)
	{
		//make sure we have at least 2 points
		if (points.size() < 2) return;

		MoveToEx(m_hdc, (int)points[0].x, (int)points[0].y, NULL);

		for (unsigned int p=1; p<points.size(); ++p)
		{
			LineTo(m_hdc, (int)points[p].x, (int)points[p].y);
		}
	}

	void LineWithArrow(Vector2D from, Vector2D to, double size)
	{
		Vector2D norm = Vec2DNormalize(to-from);

		//calculate where the arrow is attached
		Vector2D CrossingPoint = to - (norm * size);

		//calculate the two extra points required to make the arrowhead
		Vector2D ArrowPoint1 = CrossingPoint + (norm.Perp() * 0.4f * size); 
		Vector2D ArrowPoint2 = CrossingPoint - (norm.Perp() * 0.4f * size); 

		//draw the line
		MoveToEx(m_hdc, (int)from.x, (int)from.y, NULL);
		LineTo(m_hdc, (int)CrossingPoint.x, (int)CrossingPoint.y);

		//draw the arrowhead (filled with the currently selected brush)
		POINT p[3];

		p[0] = VectorToPOINT(ArrowPoint1);
		p[1] = VectorToPOINT(ArrowPoint2);
		p[2] = VectorToPOINT(to);                  

		SetPolyFillMode(m_hdc, WINDING);
		Polygon(m_hdc, p, 3);

	}

	void Cross(Vector2D pos, int diameter)
	{
		Line((int)pos.x-diameter,(int)pos.y-diameter,(int)pos.x+diameter, (int)pos.y+diameter);
		Line((int)pos.x-diameter,(int)pos.y+diameter,(int)pos.x+diameter, (int)pos.y-diameter);
	}


	//---------------------Geometry drawing methods

	void Rect(int left, int top, int right, int bot)
	{
		Rectangle(m_hdc, left, top, right, bot);
	}

	void Rect(double left, double top, double right, double bot)
	{
		Rectangle(m_hdc, (int)left, (int)top, (int)right, (int)bot);
	}



	void ClosedShape(const std::vector<Vector2D> &points)
	{
		MoveToEx(m_hdc, (int)points[0].x, (int)points[0].y, NULL);

		for (unsigned int p=1; p<points.size(); ++p)
		{
			LineTo(m_hdc, (int)points[p].x, (int)points[p].y);
		}

		LineTo(m_hdc, (int)points[0].x, (int)points[0].y);
	}


	void Circle(Vector2D pos, double radius)
	{
		Ellipse(m_hdc,
			(int)(pos.x-radius),
			(int)(pos.y-radius),
			(int)(pos.x+radius+1),
			(int)(pos.y+radius+1));
	}

	void Circle(double x, double y, double radius)
	{
		Ellipse(m_hdc,
			(int)(x-radius),
			(int)(y-radius),
			(int)(x+radius+1),
			(int)(y+radius+1));
	}

	void Circle(int x, int y, double radius)
	{
		Ellipse(m_hdc,
			(int)(x-radius),
			(int)(y-radius),
			(int)(x+radius+1),
			(int)(y+radius+1));
	}


	void SetPenColor(int color)
	{
		assert (color < SNumber);

		switch (color)
		{
		case black: BlackPen(); return;
		case white: WhitePen(); return;
		case red:   RedPen();   return;
		case green: GreenPen(); return;
		case blue:  BluePen(); return;
		case pink:  PinkPen(); return;
		case grey:  GreyPen(); return;
		case yellow: YellowPen(); return;
		case orange: OrangePen(); return;
		case purple: PurplePen(); return;
		case brown:  BrownPen();  return;
		case light_blue: LightBluePen(); return;
		case light_grey: LightGreyPen(); return;
		case light_pink: LightPinkPen(); return;
		}
	}

	void  HatchRBrush()
	{
		m_whiteBrush = CreateHatchBrush(HS_FDIAGONAL, RGB(255, 255, 255)); 
		SelectObject(m_hdc,m_whiteBrush);
	}

	void  HatchLBrush()
	{
		m_whiteBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 255, 255)); 
		SelectObject(m_hdc,m_whiteBrush);
	}

	void  HatchXBrush()
	{
		m_whiteBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 255, 255)); 
		SelectObject(m_hdc,m_whiteBrush);
	}

	void  ClearBrush()
	{
		DeleteObject(m_whiteBrush);
		//m_whiteBrush = CreateSolidBrush(RGB(255, 255, 255)); 
		//SelectObject(m_hdc,m_whiteBrush);
		/*//DeleteObject(m_whitePen);
		
		m_whiteBrush=CreateSolidBrush(colors[white]);*/
		SelectObject(m_hdc,GetStockObject(NULL_BRUSH)); //ע�⣺ѡ�ÿջ�ˢ

		
	}
};
