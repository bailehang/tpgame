#pragma once

#include "State.h"

class CState;

template < class  entity_type>
class  CStateMachine
{
	typedef   entity_type					Entity;

private:
	Entity*				m_pOwner;
	
	CState*				m_pCurrentState;

	CState*				m_pPreviousState;

public:
	CStateMachine( Entity* pOwner ) : m_pOwner ( pOwner )
	{
		m_pPreviousState = m_pPreviousState = NULL;
	}

	virtual ~CStateMachine()	{	}

	void	SetCurrentState(CState* cs)	
	{
		m_pCurrentState = cs;
	}

	void	SetPreviousState(CState* cs)
	{
		m_pPreviousState= cs;
	}

	void	Update() const 
	{
		if ( m_pCurrentState != NULL )
		{
			m_pCurrentState->Execute( m_pOwner );
		}
	}

	bool	HandleEvent( eEventAi& e )
	{

		return true;
	}

	void    ChangeState(CState*  pNewState)
	{
		assert( pNewState );
		
		if ( pNewState != m_pCurrentState )
		{
			m_pPreviousState = m_pCurrentState;
			m_pCurrentState->Exit( m_pOwner );
			m_pCurrentState->Enter(m_pOwner );
			m_pCurrentState->Exectue(m_pOwner);	
		}
	}

	void	RevertToPreviousState()
	{
		ChangeState( m_pPreviousState );
	}

	bool 	HandleEvent(const tagEevent  e  )
	{
		//assert( m_pCurrentState );
		return m_pCurrentState->HandleEvent( e.eEvent , m_pOwner );
	}
};
