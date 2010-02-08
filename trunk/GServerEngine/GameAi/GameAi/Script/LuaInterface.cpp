#include "stdafx.h"
#include "LuaInterface.h"

namespace  tp_script
{

	CLuaInterface::CLuaInterface()
	{
		m_pOwner = NULL;
	}

	CLuaInterface::~CLuaInterface()
	{
		Destroy();
	}

	void  CLuaInterface::Init()
	{
		m_MainState = lua_open(100);
		luaL_openlibs( m_MainState );

		/// ×¢²áº¯Êý
		m_LuaFnReg.SetOwner( this );
		m_LuaFnReg.RegisterFun();

	}

	void  CLuaInterface::Destroy()
	{
		//m_LuaParse.Exit();
		
	}

	CLuaScript*  CLuaInterface::Create()
	{
		CLuaScript* pluascript = new CLuaScript();

		pluascript->Init( m_MainState );

	    m_Scriptt[ pluascript ] =	 pluascript->m_LuaState;
		return  pluascript;
	}

	bool  CLuaInterface::FindSymbol(lua_State *L, char* funcname)
	{
		lua_getglobal( L , funcname );
		if(lua_isnil(L,-1))
		{
			lua_pop(L, 1);  
			return false;
		}
		else
		{
			lua_pop(L, 1);  
			return true;
		}
		return false;
	}

	bool  CLuaInterface::PrefixCall(lua_State *L, char** funcname)
	{
		if(!FindSymbol(L,*funcname))
		{
			char* pSrc = strchr(*funcname,'_')+1;
			memmove(*funcname,pSrc,strlen(pSrc)+1);
			if(!FindSymbol(L,*funcname))
			{
			    return false;
			}
		}
		return true;
	}

	int  CLuaInterface::ExeFile(char *filename, char *funcname, bool bload)
	{
		CLuaScript *lua = Create();
		if ( !lua )
			return 0;

 		if( bload )
 			bool  ret =  lua->Load( filename );
		lua->ExecuteCode();

		if( funcname !=NULL)
 		  bool  ret = lua->CallFunction( funcname , 1, "" );
		return 0;
	}

	int  CLuaInterface::ExeScript(int sid, char *funcname)
	{
		return	 ExeFile(funcname,"test1",true);
	}

	int  CLuaInterface::ExeScript(int sid, char *funcname, int Param0 )
	{
	   return	 ExeFile("",funcname,true);
	}


}