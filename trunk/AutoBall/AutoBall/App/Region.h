#ifndef REGION_H
#define REGION_H

#include <math.h>

#include "../Render/Vector2D.h"
#include "../Render/VGdi.h"
#include "../Render/Utils.h"
#include "../Public/Singleton.h"
#include "../Public/Stream_Utility_Fun.h"

class Region
{
public:
	enum region_modifier{halfsize, normal};

public:

	Region():m_dTop(0),m_dBottom(0),m_dLeft(0),m_dRight(0)
	{}


	Region(double left,
			double top,
			double right,
			double bottom,
			int id = -1):m_dTop(top),
			m_dRight(right),
			m_dLeft(left),
			m_dBottom(bottom),
			m_iID(id)
	{
		m_vCenter = Vector2D( (left+right)*0.5, (top+bottom)*0.5 );

		m_dWidth  = fabs(right-left);
		m_dHeight = fabs(bottom-top);
	}

	virtual ~Region(){}

	virtual inline void     Render(bool ShowID)const;

	/// 判断pos是否在当前矩形区域内
	inline bool     Inside(Vector2D pos, region_modifier r)const;

	/// 在当前矩形区域，返回一个随机位置信息
	inline	   Vector2D GetRandomPosition()const;

	double     Top()const{return m_dTop;}
	double     Bottom()const{return m_dBottom;}
	double     Left()const{return m_dLeft;}
	double     Right()const{return m_dRight;}
	double     Width()const{return fabs(m_dRight - m_dLeft);}
	double     Height()const{return fabs(m_dTop - m_dBottom);}
	double     Length()const{return max(Width(), Height());}
	double     Breadth()const{return min(Width(), Height());}

	Vector2D   Center()const{return m_vCenter;}
	int        ID()const	{return m_iID;}

protected:

	/// 前后左右，坐标
	double        m_dTop;
	double        m_dLeft;
	double        m_dRight;
	double        m_dBottom;
	/// 长宽
	double        m_dWidth;
	double        m_dHeight;
	/// 场景区域的中间位置
	Vector2D     m_vCenter;
    /// 矩形对应ID
	int          m_iID;

};



inline Vector2D Region::GetRandomPosition()const
{
	return Vector2D(RandInRange(m_dLeft, m_dRight),
		RandInRange(m_dTop, m_dBottom));
}

inline bool Region::Inside(Vector2D pos, region_modifier r=normal)const
{
	if (r == normal)
	{
		return ((pos.x > m_dLeft) && (pos.x < m_dRight) &&
			(pos.y > m_dTop) && (pos.y < m_dBottom));
	}
	else
	{
		const double marginX = m_dWidth * 0.25;
		const double marginY = m_dHeight * 0.25;

		return ((pos.x > (m_dLeft+marginX)) && (pos.x < (m_dRight-marginX)) &&
			(pos.y > (m_dTop+marginY)) && (pos.y < (m_dBottom-marginY)));
	}

}

inline void Region::Render(bool ShowID = 0)const
{
	GetInstObj(CGDI).HollowBrush();
	GetInstObj(CGDI).GreenPen();
	GetInstObj(CGDI).Rect(m_dLeft, m_dTop, m_dRight, m_dBottom);

	if (ShowID)
	{ 
		GetInstObj(CGDI).TextColor(CGDI::green);
		GetInstObj(CGDI).TextAtPos(Center(), ttos(ID()));
	}
}

#endif
