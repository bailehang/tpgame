#pragma once

#include "Obj.h"
#include "Region.h"

class CObj_MoveShape :
	public CObj
{
	typedef		AI_SYSTEM::CRegion	CRegion;

public:
	CObj_MoveShape(void);

	~CObj_MoveShape(void);

	virtual	 void		Init();

	virtual	 void		Clear();


	/*
	 *	Event Handler
	 */
public:
	virtual	 void		OnDie();
	virtual	 void		OnRelive();
	virtual  void		OnBeenKilled() ;
	
	

	/*
	 *	Action
	 */
public:
	virtual	 bool		StarAi( int	uTimer =0 );
	
	/*
	 *	operator 
	 */
public:
	CRegion *			GetPoint	();

private:
	

};
