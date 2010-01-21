#pragma  once

#include "BaseDef.h"

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

	virtual	  void  HandleEvent( eEventAi& e ) = 0;

	virtual	  ~CBaseGameEntity() {}
};