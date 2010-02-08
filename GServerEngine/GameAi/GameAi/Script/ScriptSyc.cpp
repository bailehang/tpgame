#include "stdafx.h"
#include "ScriptSyc.h"


namespace tp_script
{
	ScriptSyc::ScriptSyc()
	{
		m_Scriptt.clear();
	}

	ScriptSyc::~ScriptSyc()
	{
		m_Scriptt.clear();
	}

	void ScriptSyc::Init()
	{
		m_MainState = lua_open(0);
		luaL_openlibs( m_MainState );
	}

}

