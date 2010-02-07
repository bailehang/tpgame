#include "stdafx.h"
#include "LuaFnRegister.h"
#include "LuaFunc/LuaFnTest.h"
#include "LuaInterface.h"

namespace tp_script
{
	
	struct   reg_luafun  functbl []	=
	{
		{"test1" , func(luatest1) } ,
		{"test2" , func(luatest2) } ,
	};

	CLuaFnRegister::CLuaFnRegister()
	{
		m_pOwner = NULL;
	}

	CLuaFnRegister::~CLuaFnRegister()
	{
		//pOwner = NULL;
	}

	void  CLuaFnRegister::RegisterFun()
	{
		size_t  size = sizeof(functbl) / sizeof(reg_luafun);
		for ( size_t  i = 0  ; i < size ; i++ )
		{	
			m_pOwner->m_LuaParse.RegFunction( tp_script::functbl[i].name , tp_script::functbl[i].func );
		}
	}

	void  CLuaFnRegister::SetOwner( CLuaInterface* pOwner )
	{
		m_pOwner = pOwner;
	}

	CLuaInterface* CLuaFnRegister::GetOwner() const 
	{
		return m_pOwner;
	}
}