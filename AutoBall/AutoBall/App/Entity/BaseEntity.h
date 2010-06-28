
#pragma once 

#include "../../Public/BaseDef.h"

class CBaseGameEntity
{
private:
	eEntityType	  eid;

public:
	CBaseGameEntity( eEntityType  e)	: eid( e )
	{		}

	eEntityType	GetEntityType()		
	{
		return  eid;
	}

	virtual   void  Update() = 0 ;

	virtual	  void  HandleEvent(const tagEevent& e );

	virtual	  ~CBaseGameEntity() {}
};