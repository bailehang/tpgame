//#include "stdafx.h"
#include "VGdi.h"

CGDI* CGDI::Instance()
{
	static CGDI instance;
	return &instance;
}

CGDI::CGDI()
{
	m_blackPen = CreatePen(PS_SOLID, 1, colors[black]);
	m_whitePen = CreatePen(PS_SOLID, 1, colors[white]);
	m_redPen = CreatePen(PS_SOLID, 1, colors[red]);
	m_greenPen = CreatePen(PS_SOLID, 1, colors[green]);
	m_bluePen = CreatePen(PS_SOLID, 1, colors[blue]);
	m_greyPen = CreatePen(PS_SOLID, 1, colors[grey]);
	m_pinkPen = CreatePen(PS_SOLID, 1, colors[pink]);
	m_yellowPen = CreatePen(PS_SOLID, 1, colors[yellow]);
	m_orangePen = CreatePen(PS_SOLID, 1, colors[orange]);
	m_purplePen = CreatePen(PS_SOLID, 1, colors[purple]);
	m_brownPen = CreatePen(PS_SOLID, 1, colors[brown]);

	m_darkgreenPen = CreatePen(PS_SOLID, 1, colors[dark_green]);

	m_lightbluePen = CreatePen(PS_SOLID, 1, colors[light_blue]);
	m_lightgreyPen = CreatePen(PS_SOLID, 1, colors[light_grey]);
	m_lightPinkPen = CreatePen(PS_SOLID, 1, colors[light_pink]);

	m_thickblackPen = CreatePen(PS_SOLID, 2, colors[black]);
	m_thickwhitePen = CreatePen(PS_SOLID, 2, colors[white]);
	m_thickredPen = CreatePen(PS_SOLID, 2, colors[red]);
	m_thickgreenPen = CreatePen(PS_SOLID, 2, colors[green]);
	m_thickbluePen = CreatePen(PS_SOLID, 2, colors[blue]);

	m_redBrush = CreateSolidBrush(colors[green]);
	m_greenBrush   = CreateSolidBrush(colors[red]);
	m_blueBrush  = CreateSolidBrush(colors[blue]);
	m_greyBrush  = CreateSolidBrush(colors[grey]);
	m_brownBrush = CreateSolidBrush(colors[brown]);
	m_yellowBrush = CreateSolidBrush(colors[yellow]);
	m_lightblueBrush = CreateSolidBrush(RGB(0,255,255));
	m_darkgreenBrush = CreateSolidBrush(colors[dark_green]);
	m_orangeBrush = CreateSolidBrush(colors[orange]);

	m_hdc = NULL;
}

CGDI::~CGDI()
{
	DeleteObject(m_blackPen);
	DeleteObject(m_whitePen);
	DeleteObject(m_redPen);
	DeleteObject(m_greenPen);
	DeleteObject(m_bluePen);
	DeleteObject(m_greyPen);
	DeleteObject(m_pinkPen);
	DeleteObject(m_orangePen);
	DeleteObject(m_yellowPen);
	DeleteObject(m_purplePen);
	DeleteObject(m_brownPen);
	DeleteObject(m_OldPen);

	DeleteObject(m_darkgreenPen);

	DeleteObject(m_lightbluePen);
	DeleteObject(m_lightgreyPen);
	DeleteObject(m_lightPinkPen);

	DeleteObject(m_thickblackPen);
	DeleteObject(m_thickwhitePen);
	DeleteObject(m_thickredPen);
	DeleteObject(m_thickgreenPen);
	DeleteObject(m_thickbluePen);

	DeleteObject(m_greenBrush);
	DeleteObject(m_redBrush);
	DeleteObject(m_blueBrush);
	DeleteObject(m_oldBrush);
	DeleteObject(m_greyBrush);
	DeleteObject(m_brownBrush);
	DeleteObject(m_lightblueBrush);
	DeleteObject(m_yellowBrush);
	DeleteObject(m_darkgreenBrush);
	DeleteObject(m_orangeBrush);

}