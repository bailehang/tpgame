
#include "stdafx.h"
#include "BLuaScript.h"

namespace tp_script
{
	
	CBaseScript::CBaseScript()
	{

	}

	CBaseScript::~CBaseScript()
	{

	}

	bool CBaseScript::CallFunction(char* cFunName , int nResults , char* cFormat, ... )
	{

		return true;
	}

	bool CBaseScript::RegFunction(char* cFunName , void * Func )
	{

		return true;
	}
}