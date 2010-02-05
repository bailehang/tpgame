/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/4]
*/

#pragma  once

#include "LuaScript.h"
#include "InterfaceLua.h"


namespace tp_script
{

typedef int (* func)( lua_State * );
#define regluafunction( funcname ) {#funcname, func(funcname) } ;

	class CInterfaceLua;


	class CLuaFnRegister
	{

	private:

		// CLuaScript  
		CInterfaceLua * pOwner;

	public:

		CLuaFnRegister();

		~CLuaFnRegister();

		void   SetOwner( CInterfaceLua* pOwner ); 

		CInterfaceLua* GetOwner();

		void   RegisterFun();

	};
	


}