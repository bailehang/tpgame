/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/8]
*/

#pragma  once

namespace tp_script
{
	class  ScriptSyc
	{
		typedef  std::map<long,CLuaScript*>   ScriptTable;


	private:
		lua_State*  m_MainState;
		ScriptTable m_Scriptt;

	public:

		ScriptSyc();

		~ScriptSyc();

		///  init 
		void   Init( ) ;

		///  Destroy
		void   Destroy() ;

	};
}