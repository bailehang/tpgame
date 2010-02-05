#include "LuaFnRegister.h"


namespace tp_script
{
	
	struct   reg_luafun  functbl []
	{
		{"test1" , func(luatest1) } ,
		{"test2" , func(luatest2) } ,
	};

	CLuaFnRegister::CLuaFnRegister()
	{
		pOwner = NULL;
	}

	CLuaFnRegister::~CLuaFnRegister()
	{
		pOwner = NULL;
	}

	void  CLuaFnRegister::RegisterFun()
	{
		size_t  size = sizeof(functbl) / sizeof(reg_luafun);
		for ( size_t  i = 0  ; i < size ; i+= )
		{	
			pOwner->m_LuaParse.RegisterFunction( tp_script::functbl[i].name , tp_script::functbl[i].func );
		}
	}

	void  CLuaFnRegister::SetOwner( CInterfaceLua* pOwner )
	{
		pOwner = pOwner;
	}

	CLuaInterface* CLuaFnRegister::GetOwner() const 
	{
		return pOwner;
	}
}