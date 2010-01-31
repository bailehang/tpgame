#include "StdAfx.h"
#include "Region.h"

namespace  AI_SYSTEM
{
	#define   DIS(x,y)		(x*(m_Height-1) + y )

	CRegion::CRegion(void)
	{
		m_Width = m_Height = 0; 
		m_Point = NULL;
	}

	CRegion::~CRegion(void)
	{
		if ( m_Point != NULL )
		{
			delete [] m_Point ;
		}
	}


	void	CRegion::Init()
	{
		memset( m_Point , 0 ,sizeof(int) * m_Width * m_Height );
	}

	void	CRegion::SetPoint( long x , long y , long val )
	{
		m_Point[ DIS(x,y) ] =  val;
	}

	int		CRegion::GetPoint( long  x , long y )
	{
		return  m_Point[ DIS(x,y) ];
	}

	// int		CRegion::operator [](int x ,int y ) const
	// {
	// 	return  GetPoint(x,y);
	// }
	// 
	// int		CRegion::operator [](int x , int y , int val)
	// {
	// 	retur	SetPoint( x,y,val);

}