/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/4]
*/

#pragma  once

#include "LuaScript.h"
//#include "LuaInterface.h"


namespace tp_script
{

typedef int (* func)( lua_State * );
#define regluafunction( funcname ) {#funcname, func(funcname) } ;

	class CLuaInterface;

	class CLuaFnRegister
	{
	private:
		// CLuaScript  
		CLuaInterface* m_pOwner;

	public:

		CLuaFnRegister();

		~CLuaFnRegister();

		void   SetOwner( CLuaInterface* pOwner ); 

		CLuaInterface* GetOwner() const;

		void   RegisterFun();

	};
	


}