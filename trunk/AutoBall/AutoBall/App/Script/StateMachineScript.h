/*
 *	
 */

#pragma  once

#include <LuaPlus.h>
#include <cassert>
#include <string>
#include <luabind/luabind.hpp>
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
class  StateMachine
{

public:
	StateMachine(entity* owner):m_pOwner(owner)
	{

	}

	void  SetCurrentState(const luabind::object& s)
	{ 
		m_CurrentState = s; 
	}

	void  Update() const
	{
		if (m_CurrentState.is_valid())
			m_CurrentState["Execute"](m_pOwner);
	}

	void  ChangeState(const luabind::object& s)
	{
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

	const luabind::object&  CurrentState()const{return m_CurrentState;}

private:

	entity*          m_pOwner;
	
	luabind::object  m_CurrentState;
};