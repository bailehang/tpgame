#pragma once

#include "../Public/BaseGameEntity.h"
#include "../Public/StateMachine.h"
#include "../Public/State.h"
#include "../Public/StateSet.h"
#include "../singleton.h"

template< typename type_name>
class  CSingleton;


class  CMonsterEntity : public CBaseGameEntity
{
	typedef  CMonsterEntity   self;
private:

	// an instance of state machine class 
	CStateMachine<self>*	m_pMachine;

	//
public:
	CMonsterEntity( eEntityType  e ) : CBaseGameEntity( e )
	{
		m_pMachine = new CStateMachine<self>( this );

		m_pMachine->SetCurrentState( GetInstObj(CStateSet).GetState( Ent_PeaceState ) );
	}

	void  HandleEvent(  const tagEevent& e ) 
	{
		m_pMachine->HandleEvent( e );
	}

	void  Update()
	{
		m_pMachine->Update();
	}
};