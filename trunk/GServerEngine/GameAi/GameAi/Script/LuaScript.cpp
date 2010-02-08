#include "stdafx.h"
#include "LuaScript.h"


namespace tp_script
{

#define  LUA_OUTERRMSG(STR ) \
	fprintf( stderr, STR )


	/// 构造函数
	CLuaScript::CLuaScript()
	{
		m_LuaState  = lua_open( 100 );

		if ( m_LuaState == NULL )
		{
			ScriptError( LUA_CREATE_ERROR );
			m_IsRuning = false;
			return ;
		}

		m_IsRuning = true;
		m_szScriptName[0] = '\0';
	}

	/// 析构函数
	CLuaScript::~CLuaScript()
	{
		Exit();
	}

	bool CLuaScript::LoadBuffer(unsigned char *pBuffer, size_t dwLen)
	{
		if ( dwLen < 0 )
		{
			ScriptError( LUA_SCRIPT_LEN_ERROR );
			return false;
		}

		//prase_buffer()
		if ( luaL_loadbuffer(m_LuaState , (char*)pBuffer , dwLen , 0) != 0 )
		{
			ScriptError( LUA_SCRIPT_COMPILE_ERROR );
			return false;
		}
		return true;
	}

	bool  CLuaScript::Load(const char *FileName)
	{
		try
		{
			if( !m_LuaState )
				return false;
			if ( !luaL_loadfile(m_LuaState , FileName))
			{
				ScriptError( LUA_SCRIPT_COMPILE_ERROR );
				return false;
			}
		}
		catch( ... )
		{
			return false;
		}

		//if ( !ExecuteCode() )
		//	return false;
		return true;
	}

	/// 执行
	bool CLuaScript::Execute()
	{
		if( m_IsRuning && m_LuaState )
			return CallFunction("main", 0 , "" );
		
		return false;
	}

	/// 执行
	bool CLuaScript::ExecuteCode()
	{
		if ( !( m_IsRuning && m_LuaState ))
		{
			ScriptError( LUA_SCRIPT_EXECUTE_ERROR );
			return false;
		}

		int  state ; //lua_execute
		if ( state = lua_pcall (m_LuaState,0,LUA_MULTRET,0) != 0 )
		{
			ScriptError( LUA_SCRIPT_EXECUTE_ERROR , state );
			return false;
		}
		return true;
	}

	/**
	 * 函数:	CallFunction
	 * 功能:	调用Lua脚本内的函数
	 * 参数:	char* cFuncName
	 * 参数:	int   nResults
	 * 参数:	char* cFormat  调用时所传参数的类型
	 *			n:数字型(double) d:整形(int) s:字符串型 f:C函数型  
	            n:Nil v:Value p:Point v形为Lua支持的
	            参数为整形的数index，指明将index所指堆栈的变量作为该函数的调用参数。	

	 *	注意：由于该函数有不定参数…,对于数字，系统并不确定数是以double还是以int
	 *  存在，两种保存形式是不同的。因此需要注意当传入的数是整形时，格式符应用d
	 *  而不能用n,或者强行改变为double形。否则会出现计算的错误。  
	 */

	bool  CLuaScript::CallFunction(const char* cFuncName , int nResults ,
									char* cFormat, va_list vlist)
	{

		double nNumber ;
		char*  cString =  NULL;
		void*  pPoint  =  NULL;
		
		lua_CFunction   CFunc;
		
		int    i = 0 , nArgNum = 0 , nIndex = 0 , nRetcode = 0;

		if ( ! (m_IsRuning && m_LuaState) )
		{
			ScriptError( LUA_SCRIPT_STATES_IS_NULL  );
			return false;
		}

		lua_getglobal( m_LuaState , cFuncName );

		while ( cFormat[i] != '\0' )
		{
			switch ( cFormat[i] )
			{
				/// double 
			case 'n':
				{
					nNumber = va_arg( vlist ,double);
					lua_pushnumber( m_LuaState , nNumber );
					nArgNum ++ ;
				}
				break;

				///  int 
			case 'd':
				{
					nNumber = (double)(va_arg(vlist, int));
					lua_pushnumber(m_LuaState, (double) nNumber);
					nArgNum ++;
				}
				break;

				///  string
			case 's':
				{
					cString = va_arg(vlist, char *);
					lua_pushstring(m_LuaState, cString);
					nArgNum ++;							
				}
				break;

				/// Null
			case 'N':
				{
					lua_pushnil(m_LuaState);
					nArgNum ++;
				}
				break;

				/// call function
			case 'f':
				{
					CFunc = va_arg(vlist, lua_CFunction);
					lua_pushcfunction(m_LuaState, CFunc) ;
					nArgNum ++;
				}
				break;
				
				/// 输入的是堆栈中Index为nIndex的数据类型
			case 'v':
				{
					nNumber = va_arg(vlist, int);
					int nIndex1 = (int) nNumber;
					lua_pushvalue(m_LuaState, nIndex1);
					nArgNum ++;
				}
				break;

				/// 输入为一Table类型
			case 't':
				{

				}
				break;

			case 'p':
				{
					pPoint = va_arg(vlist, void *);

					/// Lua_PushUserTag(m_LuaState, pPoint,m_UserTag);
					nArgNum ++;
				}
				break;
			}
			i++;	
		}

		lua_call( m_LuaState , nArgNum , nResults );
		if ( nRetcode != 0 )
		{
			ScriptError( LUA_SCRIPT_EXECUTE_ERROR , nRetcode );
			return false;
		}
		return true;
	}


	bool CLuaScript::CallFunction(const char* cFuncName, int nResults, char* cFormat, ...)
	{
		bool    bResult  = false;
		va_list vlist;
		va_start(vlist, cFormat);
		bResult = CallFunction(cFuncName, nResults, cFormat, vlist);
		va_end(vlist);
		return bResult;
	}

	///
	///  CLuaScript::GetValuesFromStack 从堆栈中获得变量
	/// 
	bool CLuaScript::GetValuesFromStack(const char * cFormat, ...)	
	{
		va_list vlist;
		double* pNumber = NULL;
		const char **   pString ;
		int * pInt = NULL;
		int i = 0;
		int nTopIndex = 0;
		int nIndex = 0;	  

		/// cFormat的字符长度，表示需要取的参数数量
		int nValueNum = 0;

		if (! m_LuaState)
			return false;

		/// Lua_GetTopIndex
		nTopIndex = lua_gettop(m_LuaState);	
		nValueNum = strlen(cFormat);
		
		/// 当堆栈中无数据或不取参数是返回false
		if (nTopIndex == 0 || nValueNum == 0)
			return false;

		if (nTopIndex < nValueNum)
			return false;

		nIndex = nTopIndex - nValueNum +1;
		{
			va_start(vlist, cFormat);     
			while (cFormat[i] != '\0')
			{
				switch(cFormat[i])
				{					
					/// 返回值为数值形,Number,此时Lua只传递double形的值
				case 'n':
					{
						pNumber = va_arg(vlist, double *);
						if (pNumber == NULL)
							return false;

						if (lua_isnumber(m_LuaState, nIndex ))
						{
							* pNumber = lua_tonumber(m_LuaState, nIndex ++ );
						}
						else
						{
							ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
							return false;
						}
					}
					break;
				case 'd':
					{
						pInt = va_arg(vlist, int *);
						if (pInt == NULL)
							return false;
						if ( lua_isnumber(m_LuaState, nIndex))
						{
							* pInt = (int ) lua_tonumber(m_LuaState, nIndex ++);
						}
						else
						{
							ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
							return false;
						}
					}
					break;
				case 's'://字符串形
					{
						pString = va_arg(vlist, const char **);

						if (pString == NULL)
							return false;

						if (lua_isstring(m_LuaState, nIndex))
						{
							(*pString) = (const char *)lua_tostring(m_LuaState, nIndex++);
						}
						else
						{
							ScriptError(LUA_SCRIPT_NOT_STRING_ERROR);
							return false;
						}
					}
					break;
				}

				i ++;	
			}
			va_end(vlist);     		/* Reset variable arguments.      */

		}
		return	true;
	}

	/// 初始化脚本对象，注册系统标准函数库
	bool CLuaScript::Init( lua_State* LuaMain)
	{
		if (! m_LuaState)
		{
			m_LuaState				= lua_newthread( LuaMain );

			if (m_LuaState == NULL)
			{
				ScriptError(LUA_CREATE_ERROR);
				m_IsRuning			= false;
				return false;
			}

			m_IsRuning				= true;
			m_szScriptName[0]		= '\0';
			//m_UserTag = lua_newtag(m_LuaState)	;
		}

		RegisterStandardFunctions();
		return	true;
	}

	//---------------------------------------------------------------------------
	// 函数:	CLuaScript::RegisterFunction
	// 功能:	注册某内部C函数至脚本中
	// 参数:	char* FuncName  在脚本中使用的函数名
	// 参数:	void* Func    实际相应的C函数指针
	// 参数:	int Args = 0 //与KScript接口相容，无用
	// 参数:	int Flag = 0 //与KScript接口相容, 无用
	//---------------------------------------------------------------------------
	bool CLuaScript::RegFunction(const char* FuncName , void* Func)
	{
		if (! m_LuaState)
			return false;
		lua_register(m_LuaState, FuncName, (lua_CFunction)Func);
		return true;
	}

	bool CLuaScript::Compile(const char *filename)
	{
		Load(filename);
		return true;
	}

	// 功能:	批量注册Lua的内部C函数，各个函数的信息保存在reg_luafun的数据中
	// 参数:	reg_luafun *Funcs 数组的指针
	// 参数:	int n 函数数量。可以为零，由系统计算得到。
	bool CLuaScript::RegisterFun(reg_luafun Funcs[], int n)
	{
		if (! m_LuaState)	return false;
		if (n == 0)	n = sizeof(Funcs) / sizeof(Funcs[0]);
		for (int i = 0; i < n; i ++)	lua_register(m_LuaState, Funcs[i].name, Funcs[i].func);
		return true;
	}

	/// 注册Lua系统标准的函数库
	void CLuaScript::RegisterStandardFunctions()
	{
		if (! m_LuaState)		return ;
		luaL_openlibs( m_LuaState );  

		/*
// 		lua_baselibopen(m_LuaState);//Lua基本库
// 		Lua_OpenIOLib(m_LuaState);//输入输出库
// 		Lua_OpenStrLib(m_LuaState);//字符串处理库
// 		Lua_OpenMathLib(m_LuaState);//数值运算库
// 		//Lua_OpenDBLib(m_LuaState);//调试库
*/
		return;	
	}


	//---------------------------------------------------------------------------
	// 函数:	CLuaScript::ReleaseScript
	// 功能:	释放该脚本资源。
	// 返回:	bool 
	//---------------------------------------------------------------------------
	void CLuaScript::Exit()
	{
		if (! m_LuaState)		return ;
		lua_close(m_LuaState);
		m_LuaState = NULL;
		m_IsRuning = false;
	}

	template < typename  type>
	type CLuaScript::PopLuaNumber( const char* vPar)
	{
		lua_settop( m_LuaState , 0 );

		lua_getglobal( m_LuaState, vPar);

		if ( !lua_isnumber(m_LuaState ,1) )
		{
			ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
			return 0;
		}

		type val = (type)lua_tonumber(  m_LuaState , 1 );

		lua_pop(m_LuaState , 1 );

		return val;
	}

	std::string CLuaScript::PopLuaString( const char* vPar)
	{
		lua_settop( m_LuaState , 0 );

		lua_getglobal( m_LuaState, vPar);

		if ( !lua_isstring(m_LuaState ,1) )
		{
			ScriptError(LUA_SCRIPT_NOT_STRING_ERROR);
			return 0;
		}

		std::string str = lua_tostring(  m_LuaState , 1 );

		lua_pop(m_LuaState , 1 );

		return str;
	}

	bool CLuaScript::PopLuaBoolean( const char* vPar)
	{
		lua_settop( m_LuaState , 0 );

		lua_getglobal( m_LuaState, vPar);

		if ( !lua_isboolean(m_LuaState ,1) )
		{
			ScriptError(LUA_SCRIPT_NOT_BOOLEN_ERROR);
			return 0;
		}

		bool val = (bool)lua_toboolean( m_LuaState , 1 );

		lua_pop(m_LuaState , 1 );

		return val;
	}

	void CLuaScript::ScriptError(int Error)
	{
		char lszErrMsg[200];
		sprintf(lszErrMsg, "ScriptError %d. (%s) \n", Error, m_szScriptName);
		LUA_OUTERRMSG(lszErrMsg);
		return;
	}

	void CLuaScript::ScriptError(int Error1 ,int Error2)
	{
		char lszErrMsg[200];
		sprintf(lszErrMsg, "ScriptError %d:[%d] (%s) \n", Error1, Error2, m_szScriptName);
		LUA_OUTERRMSG(lszErrMsg);
		return;
	}

	bool CLuaScript::RunLuaSrcipt(const char* FileName)
	{
		if (int error = luaL_dofile(m_LuaState, FileName) != 0)
		{
			//throw std::runtime_error("ERROR(" + ttos(error) + "): Problem with lua script file " + FileName);
			char lszErrMsg[200];
			sprintf(lszErrMsg, "runtime_error %d (%s) \n", error,  FileName);
			LUA_OUTERRMSG(lszErrMsg);
			return false;
		}
		return true;
	}

	//---------------------------------------------------------------------------
	// 函数:	CLuaScript::SafeCallBegin
	// SafeCallBegin与SafeCallEnd两函数应搭配使用，以防止在调用Lua的外部函数之后，
	// 有多余数据在堆栈中未被清除。达到调用前与调用后堆栈的占用大小不变。
	// 上述情况只需用在调用外部函数时，内部函数不需如此处理。
	void CLuaScript::SafeCallBegin(int * pIndex)
	{
		if (! m_LuaState)		return ;
		//lua_gettopindex(m_LuaState, pIndex);
	}

	void CLuaScript::SafeCallEnd(int nIndex)
	{
		if (! m_LuaState)	return;
		//Lua_SafeEnd(m_LuaState, nIndex);
	}

	bool CLuaScript::Stop(void)
	{
		if (! m_IsRuning)		return true;
		if (! m_LuaState)		return false;
		m_IsRuning =  false;
		return true;
	}

	// 函数:	CLuaScript::ResumeScript
	// 功能:	恢复已中止的脚本
	bool CLuaScript::Resume(void)
	{
// 		if ((! m_IsRuning) && (m_LuaState))
// 		{
// 			m_IsRuning = true;
// 			return true;
// 		}
// 		return false;

		return lua_resume( m_LuaState, 0 );
	}


	// 功能:	建立一个Lua的Table，在调用该函数并设置Table各个成员之后，必须调用
	//			SetGlobalName()来给这个Table指定一个名字。
	// 返回:	size_t 
	size_t CLuaScript::CreateTable()
	{
		int nIndex = 0;

		nIndex = lua_gettop(m_LuaState) ;
		lua_newtable(m_LuaState);
		if (lua_gettop(m_LuaState) != ++nIndex ) 
			return -1;

		return nIndex;
	}

	void CLuaScript::SetGlobalName(const char* szName)
	{
		if (!szName) return ;
		lua_setglobal(m_LuaState, szName);
	}

	size_t CLuaScript::ModifyTable(const char* szTableName) 
	{
		if (! szTableName[0])		return -1;

		int nIndex = lua_gettop(m_LuaState);

		lua_gettable(m_LuaState, nIndex);

		if (lua_gettop(m_LuaState) != ++nIndex)		return -1;

		return nIndex;
	}

}