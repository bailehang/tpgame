#pragma once

template < class  entity_type>
class  CStateMachine
{
	typedef   entity_type   Entity;

private:
	Entity*				m_pOwner;
	
	CState<Entity>*		m_pCurrentState;

	CState<Entity>*		m_pPreviousState;

public:
	CStateMachine( Entity* pOwner ) : m_pOwner ( pOwner )
	{
		m_pPreviousState = m_pPreviousState = NULL;
	}

	virtual ~CStateMachine()	{	}

	void	SetCurrentState(CState<Entity>* cs)	
	{
		m_pCurrentState = cs;
	}

	void	SetPreviousState(CState<Entity>* cs)
	{
		m_pPreviousState= cs;
	}

	void	Update() const 
	{
		if ( m_pCurrentState ! NULL )
		{
			m_pCurrentState->Exectue( m_pOwner );
		}
	}

	bool	HandleEvent( eEventAi& e )
	{

		return true;
	}

	void    ChangeState(CState<Entity>*  pNewState)
	{
		assert( pNewState );
		
		if ( pNewState != m_pCurrentState )
		{
			m_pCurrentState->Exit( m_pOwner );
			m_pCurrentState->Enter(m_pOwner );
			m_pCurrentState->Exectue(m_pOwner);

			m_pPreviousState =
		}
	}

	void	RevertToPreviousState()
	{
		ChangeState( m_pPreviousState );
	}

};
