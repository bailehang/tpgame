#include "stdafx.h"
#include "Event.h"


void  CEventDispatch::DealEvent(const tagEevent& Event)
{
	static_cast<CBaseGameEntity*>( Event.Sender )->HandleEvent( Event );
}

void  CEventDispatch::RegEvent(const tagEevent&  Event)
{
	m_EventList.push_back( Event );
}

void  CEventDispatch::DispacthEevent()
{
	if ( !m_EventList.empty() )
	{
		tagEevent Event = m_EventList[0];
	
		DealEvent( Event ) ;
		
		m_EventList.erase( m_EventList.begin() );
	}
}