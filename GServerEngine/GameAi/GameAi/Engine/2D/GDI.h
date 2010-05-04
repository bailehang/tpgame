
/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/25]
*/

#pragma  once

#include "../Public/Vector2D.h"


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
	RGB(0, 100, 0),        //dark green
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

	HDC     m_hdc;

//
public:
	CGDI()  ;
	~CGDI() ;
	CGDI(const CGDI& );

	/// pen
	void   BlackPen() { if(m_hdc) SelectObject( m_hdc , m_blackPen) ; }
	void   RedPen()   { if(m_hdc) SelectObject( m_hdc , m_redPen)   ; }
	void   GreenPen() { if(m_hdc) SelectObject( m_hdc , m_greenPen) ; }
	void   BluePen()  { if(m_hdc) SelectObject( m_hdc , m_bluePen)  ; }
	void   PinkPen()  { if(m_hdc) SelectObject( m_hdc , m_pinkPen)  ; }
	void   GreyPen()  { if(m_hdc) SelectObject( m_hdc , m_greyPen)  ; }
	void  YellowPen() { if(m_hdc) SelectObject( m_hdc , m_yellowPen); }
	void  OrangePen() { if(m_hdc) SelectObject( m_hdc , m_orangePen); }
	void  PurplePen() { if(m_hdc) SelectObject( m_hdc , m_purplePen); }
	void  BrownPen()  { if(m_hdc) SelectObject( m_hdc , m_brownPen) ; }
	void  WhitePen()  { if(m_hdc) SelectObject( m_hdc , m_whitePen) ; }
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


	public:

};

