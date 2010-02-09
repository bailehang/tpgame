/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/4]
*/

#pragma  once

#include "LuaScript.h"


namespace tp_script
{

typedef int (* func)( lua_State * );
#define regluafunction( funcname ) {#funcname, func(funcname) } ;

	class CLuaInterface;

	///
	/// singleton
	/// 
	class CLuaFnRegister
	{
		typedef std::map<std::string, CLuaScript* >   StrLuaScriptT;

		typedef StrLuaScriptT::iterator               StrFScriptItr;

	private:
		// CLuaScript  
		CLuaInterface* m_pOwner;

		StrLuaScriptT  m_LuaRunTable;

	public:

		CLuaFnRegister();

		~CLuaFnRegister();

		void   SetOwner( CLuaInterface* pOwner ); 

		CLuaInterface* GetOwner() const;

		void   RegisterFun();

		bool   RegisterFun(reg_luafun Funcs[], size_t n);

		void   AddScript(std::string str , tp_script::CLuaScript* );
		
		//CLuaScript*  GetScript(std::string  str );
		tp_script::CLuaScript*  GetScript(std::string str);

	};
	


}