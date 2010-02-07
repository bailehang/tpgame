/*
* 
*
*/	
#pragma  once

namespace  tp_script
{

	enum  eLuaError
	{
		 LUA_CREATE_ERROR			= 1,
		 LUA_SCRIPT_LEN_ERROR		= 2,
		 LUA_SCRIPT_COMPILE_ERROR	= 3,
		 LUA_SCRIPT_EXECUTE_ERROR	= 4,
		 LUA_SCRIPT_NOT_NUMBER_ERROR=10,
		 LUA_SCRIPT_NOT_STRING_ERROR=11,
		 LUA_SCRIPT_NOT_TABLE_ERROR =12,
		 LUA_SCRIPT_STATES_IS_NULL  =20
	};

	class   CBaseScript
	{
	public:
		CBaseScript(); 
		virtual ~CBaseScript();

		virtual bool  Init()	{  return  true  ; }

		virtual void  Exit()	{  return		 ; }

		virtual bool  Load(const char* FileName )	{  return  true  ; }

		virtual bool  Compile(const char* FileName)	{  return  true  ; }

		virtual bool  Execute()						{  return  true  ; }

		virtual bool  CallFunction(char* cFunName , int nResults , char* cFormat, ... );

		virtual bool  RegFunction(char* cFunName , void * Func ) ;
	};

}