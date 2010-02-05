/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/4]
*/

#pragma  once

#include "LuaFnRegister.h"
#include "itable.h"

namespace tp_script
{
	
	class  CLuaFnRegister; 

	class  CLuaInterface
	{
	private:

		/// 脚本引擎
		CLuaScript		m_LuaParse;

		/// Lua注册类
		CLuaFnRegister  m_LuaFnReg;

		/// 脚本列表
		TableList		m_ScriptTable;

	public:

	};

}