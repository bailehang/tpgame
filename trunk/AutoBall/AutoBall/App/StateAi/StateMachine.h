
#pragma once


#include <cassert>
#include <string>

#include "State.h"
#include "../../Public/MsgImpl.h"

template <class entity_type>
class StateMachine
{
public:

	StateMachine(entity_type* owner):m_pOwner(owner),
		m_pCurrentState(NULL),
		m_pPreviousState(NULL),
		m_pGlobalState(NULL)
	{}

	~StateMachine() { }

	void SetCurrentState(State<entity_type>* s) {m_pCurrentState = s;}
	void SetGlobalState(State<entity_type>* s)  {m_pGlobalState  = s;}
	void SetPreviousState(State<entity_type>* s){m_pPreviousState= s;}

	void  Update()const
	{
		if(m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);

		if(m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}

	bool  HandleMessage(const tagMessage& msg)const
	{
		if (m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, msg))
		{
			return true;
		}

		if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
		{
			return true;
		}

		return false;
	}

	void  ChangeState(State<entity_type>* pNewState)
	{
		assert(pNewState && "<StateMachine::ChangeState>:trying to assign null state to current");

		m_pPreviousState = m_pCurrentState;

		m_pCurrentState->Exit(m_pOwner);

		m_pCurrentState = pNewState;

		m_pCurrentState->Enter(m_pOwner);
	}

	void  RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	bool  isInState(const State<entity_type>& st)const
	{
		if (typeid(*m_pCurrentState) == typeid(st)) return true;
		return false;
	}

	State<entity_type>*  CurrentState()  const{return m_pCurrentState;}
	State<entity_type>*  GlobalState()   const{return m_pGlobalState;}
	State<entity_type>*  PreviousState() const{return m_pPreviousState;}

	std::string         GetNameOfCurrentState()const
	{
		std::string s(typeid(*m_pCurrentState).name());

		if (s.size() > 5)
		{
			s.erase(0, 6);
		}

		return s;
	}

private:

	entity_type*          m_pOwner;
	State<entity_type>*   m_pCurrentState;
	State<entity_type>*   m_pPreviousState;
	State<entity_type>*   m_pGlobalState;

};

