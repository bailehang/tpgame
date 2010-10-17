/*
 *	
 */

#pragma  once

#include <LuaPlus.h>
#include <cassert>
#include <string>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <iostream>

extern "C"
{
#include <lua.h>
#include <Lualib.h>
#include <lauxlib.h>
};

using namespace luabind;
using namespace std;

#include "../../Public/MsgImpl.h"

template < class entity>
class  StateMachineScript
{

public:
	StateMachineScript(entity* owner):m_pOwner(owner)
	{

	}

	void  SetCurrentState(const luabind::object& s)
	{ 
		m_CurrentState = s; 
	}

	void  SetPreviousState(const luabind::object& s)
	{ 
		m_pPreviousState = s; 
	}

	void  SetGlobalState(const luabind::object& s)
	{ 
		m_pGlobalState = s; 
	}

	void  Update() const
	{
		if (m_pGlobalState.is_valid())
			m_pGlobalState["Execute"](m_pOwner);

		if (m_CurrentState.is_valid())
			m_CurrentState["Execute"](m_pOwner);

	}

	bool  HandleMessage(const tagMessage& msg)const
	{
		if (m_CurrentState && call_function<bool>(m_CurrentState,"OnMessage",m_pOwner, msg)[ adopt(_2) ])
		{
			return true;
		}

		if (m_pGlobalState && call_function<bool>(m_pGlobalState,"OnMessage",m_pOwner, msg)[ adopt(_2) ])
		{
			return true;
		}

		return false;
	}

	void  ChangeState(const luabind::object& s)
	{
		m_pPreviousState = m_CurrentState;

		m_CurrentState["Exit"](m_pOwner);

		m_CurrentState = s;

		m_CurrentState["Enter"](m_pOwner);
	}

	bool  isInState(const luabind::object& st)const
	{
		if( m_CurrentState == st )
			return true;
		return false;
	}

	void  EnterCurrentState() 
	{
		m_CurrentState["Enter"](m_pOwner);
	}

	const luabind::object&  CurrentState()const{return m_CurrentState;}

private:

	entity*          m_pOwner;
	
	luabind::object   m_CurrentState;
	luabind::object   m_pPreviousState;
	luabind::object   m_pGlobalState;
};