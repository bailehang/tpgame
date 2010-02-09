#include "stdafx.h"
#include "LuaInterface.h"

namespace  tp_script
{

	CLuaInterface::CLuaInterface()
	{
		m_pOwner = NULL;
		m_Scriptt.clear();
	}

	CLuaInterface::~CLuaInterface()
	{
		for (ScriptIter itr =  m_Scriptt.begin(); itr != m_Scriptt.end() ; itr ++  )
		{
			CLuaScript *lua = itr->second;
			lua->Exit();
			lua = NULL;
		}
		m_Scriptt.clear();
		lua_close(m_MainState);
		m_MainState = NULL;
	}

	void  CLuaInterface::Init()
	{
		m_MainState = lua_open(100);
		luaL_openlibs( m_MainState );

		/// ×¢²áº¯Êý
		GetInstObj(CLuaFnRegister).SetOwner( this );
		GetInstObj(CLuaFnRegister).RegisterFun();
	}

	void  CLuaInterface::Destroy(lua_State* L)
	{
		ScriptIter itr =  m_Scriptt.find( STATE_TRS(L) );
		if ( itr != m_Scriptt.end() )
		{
			CLuaScript *lua = itr->second;
			lua->Exit();
			lua = NULL;
			m_Scriptt.erase( itr->first );
		}
	}

	CLuaScript*  CLuaInterface::Create()
	{
		CLuaScript* pluascript = new CLuaScript();

		pluascript->Init( m_MainState );

	    m_Scriptt[ STATE_TRS( pluascript->m_LuaState ) ] =pluascript;

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
		CLuaScript *lua  = NULL;
		if ( (lua = GetInstObj(CLuaFnRegister).GetScript( filename) ) == NULL )
		{
			lua = Create();
		}
		
		if ( !lua )
			return 0;

 		if( bload )
 			bool  ret =  lua->Load( filename );
		int ret = lua->ExecuteCode();

		if ( ret != LUA_YIELD )
		{
			Destroy( lua->m_LuaState );
		}
// 		if( funcname !=NULL)
// 			bool  ret = lua->CallFunction( funcname , 3, "dns",123,345.0,"Ssss");
		return 0;
	}

	int  CLuaInterface::ExeScript(int sid, char *funcname)
	{
		return	 ExeFile(funcname,"test3",true);
	}

	int  CLuaInterface::ExeScript(int sid, char *funcname, int Param0 )
	{
	   return	 ExeFile("",funcname,true);
	}


}