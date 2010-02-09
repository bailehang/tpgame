#include "stdafx.h"
#include "LuaFnRegister.h"
#include "LuaFunc/LuaFnTest.h"
#include "LuaInterface.h"

namespace tp_script
{
				  
	struct   reg_luafun  functbl []	=
	{
		{"test1" , func(tp_script::luatest1) } ,
		{"test2" , func(tp_script::luatest2) } ,
		{"test3" , func(tp_script::luatest3) } ,
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
		RegisterFun( functbl , size );
	}

	void  CLuaFnRegister::SetOwner( CLuaInterface* pOwner )
	{
		m_pOwner = pOwner;
	}

	CLuaInterface* CLuaFnRegister::GetOwner() const 
	{
		return m_pOwner;
	}

	///
	/// 批量注册Lua的内部C函数，各个函数的信息保存在reg_luafun的数据中
	/// reg_luafun *Funcs 数组的指针
	/// int n 函数数量。可以为零，由系统计算得到。
	///
	bool CLuaFnRegister::RegisterFun(reg_luafun Funcs[], size_t n)
	{
		if ( !m_pOwner->m_MainState)	return false;

		if (n == 0)	
			n = sizeof(Funcs) / sizeof(Funcs[0]);

		for (size_t i = 0; i < n; i ++)	
			lua_register(m_pOwner->m_MainState, Funcs[i].name, Funcs[i].func);
		return true;
	}

	void  CLuaFnRegister::AddScript(std::string str, tp_script::CLuaScript * lua)
	{
		m_LuaRunTable[ str.c_str() ] = lua;
	}

	tp_script::CLuaScript*  CLuaFnRegister::GetScript(std::string str)
	{
		StrFScriptItr itr = m_LuaRunTable.find( str.c_str() );

		if ( itr != m_LuaRunTable.end() )
		{
			return itr->second;
		}
		return NULL;
	}

}