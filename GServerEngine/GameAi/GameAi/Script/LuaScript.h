
#pragma  once
#include "BLuaScript.h"

namespace tp_script
{
	struct reg_luafun 
	{
		const char*  name ;
		lua_CFunction func;
	};

	class  CLuaScript : public  CBaseScript
	{
	private:
		int     m_UserTag;

	public:

		CLuaScript();

		//CLuaScript( int StatckSize = 0 );

		virtual ~CLuaScript();

		int    Activate() {		Execute() ; return 1 ; }

		virtual  bool  Init( lua_State* L );

		virtual  void  Exit();

		virtual  bool  Load(const char* FileName );

		virtual  bool  Compile(const char* FileName);

		virtual  bool  Execute();

		virtual  bool  CallFunction(const char* cFuncName , int nResult ,char* cFormat, ...);

		virtual  bool  RegFunction(const char* cFuncName, void* Func);



	public:

		int		GetUserTag()		{	return  m_UserTag ;  }

		bool    RegisterFun( reg_luafun funcs[] , int  n =  0 );

		bool    LoadBuffer( unsigned char* pBuffer , size_t dwLen);

		bool    RunLuaSrcipt(const char* FileName);

		bool    GetValuesFromStack(const char* cFormat , ... );

		bool    Stop();

		bool    Resume();

		size_t  CreateTable();

		size_t  ModifyTable(const char* szTableName);

		void	SetGlobalName(const char* szName);

		inline  void SetTableMember(int nIndex , int Id , const char* szString)
		{
			// Lua_SetTable_StringFromId( m_LuaState , nIndex , Id , szString );
		}

		inline  void SetTableMember(int nIndex , int Id , double  nNumber  )
		{
			// Lua_SetTable_DoubleFromId( m_LuaState , nIndex , Id , nNumber );
		}

		inline  void SetTableMember(int nIndex , int Id , int  nNumber  )
		{
			// Lua_SetTable_IntFromId( m_LuaState , nIndex , Id , nNumber );
		}

		inline void SetTableMember(int nIndex, const char * szMemberName, lua_CFunction CFun)
		{
			//Lua_SetTable_CFunFromName(m_LuaState,  nIndex, szMemberName, CFun);
		}

		inline void SetTableMember(int nIndex, int nId, lua_CFunction CFun)
		{
			//Lua_SetTable_CFunFromId(m_LuaState,  nIndex, nId, CFun);
		}

		inline void SetTableMember(int nIndex, const char * szMemberName, char * szString)
		{
			//Lua_SetTable_StringFromName(m_LuaState,  nIndex, szMemberName, szString);
		}

		inline void SetTableMember(int nIndex, const char * szMemberName, int Number)
		{
			//Lua_SetTable_IntFromName(m_LuaState,  nIndex, szMemberName, Number);
		}

		inline void SetTableMember(int nIndex, const char * szMemberName, double Number)
		{
			//Lua_SetTable_DoubleFromName(m_LuaState,  nIndex, szMemberName, Number);
		}

		template < typename type>
		inline   type  PopLuaNumber(const char* vPar);

		inline   std::string  PopLuaString(const char* vPar);

		inline   bool    PopLuaBoolean(const char* vPar);

		void  ScriptError( int Error );

		void  ScriptError( int , int );

		int   ExecuteCode();

		void  RegisterStandardFunctions();

		bool  m_IsRuning;

		char  m_szScriptName[100];

		lua_State* m_LuaState;

	private:
		bool  CallFunction(const char* cFuncName , int nResults ,char* cFormat, va_list vlist);

	};
}