#include "stdafx.h"
#include "CLuaScript.h"


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

// 	/// 
// 	CLuaScript::CLuaScript(int StackSize = 0)
// 	{
// 		m_LuaState				= Lua_Create(StackSize);
// 
// 		if (m_LuaState == NULL )
// 		{
// 			ScriptError(LUA_CREATE_ERROR);
// 			m_IsRuning = false;
// 			return ;
// 		}
// 		m_IsRuning				= true;
// 		m_szScriptName[0]		= '\0';
// 	}

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
		//if ( lua_compilebuffer(m_LuaState , (char*) pBuffer , dwlen ) != 0 )
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
			/// read filename buffer
			/// pCon is filename context
			char *pCon = NULL ;
			size_t size= 0 ;
			if ( !LoadBuffer( pCon , size /**/) )
			{
				ScriptError( LUA_SCRIPT_COMPILE_ERROR );
				return false;
			}
		}
		catch( ... )
		{
			return false;
		}

		if ( !ExecuteCode() )
			return false;
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

		int  state ;
		if ( state = lua_execute(m_LuaState) != 0 )
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
		char*  pPoint  =  NULL;
		
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

			case 'v'://输入的是堆栈中Index为nIndex的数据类型
				{
					nNumber = va_arg(vlist, int);
					int nIndex1 = (int) nNumber;
					lua_pushvalue(m_LuaState, nIndex1);
					nArgNum ++;
				}
				break;
			case 't'://输入为一Table类型
				{

				}
				break;

			case 'p':
				{
					pPoint = va_arg(vlist, void *);

					//Lua_PushUserTag(m_LuaState, pPoint,m_UserTag);
					nArgNum ++;
				}
				break;
			}
			i++;	
		}

		nRetcode = lua_call( m_LuaState , nArgNum , nResults );
		if ( nRetcode != 0 )
		{
			ScriptError( LUA_SCRIPT_EXECUTE_ERROR , state );
			return false;
		}
		return true;
	}


	bool CLuaScript::CallFunction(const char* cFuncName, int nResults, char* cFormat, ...)
	{
		bool bResult  = false;
		va_list vlist;
		va_start(vlist, cFormat);
		bResult = CallFunction(cFuncName, nResults, cFormat, vlist);
		va_end(vlist);
		return bResult;
	}

	// 函数:	CLuaScript::GetValuesFromStack
	// 功能:	从堆栈中获得变量
	bool CLuaScript::GetValuesFromStack(char * cFormat, ...)	
	{
		va_list vlist;
		double* pNumber = NULL;
		const char **   pString ;
		int * pInt = NULL;
		int i = 0;
		int nTopIndex = 0;
		int nIndex = 0;
		int nValueNum = 0;//cFormat的字符长度，表示需要取的参数数量

		if (! m_LuaState)
			return false;

					//Lua_GetTopIndex
		nTopIndex = lua_gettop(m_LuaState);	
		nValueNum = strlen(cFormat);

		if (nTopIndex == 0 || nValueNum == 0)//当堆栈中无数据或不取参数是返回false
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
					//返回值为数值形,Number,此时Lua只传递double形的值
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
	bool CLuaScript::Init()
	{
		if (! m_LuaState)
		{
			m_LuaState				= lua_open(0);

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
	bool CLuaScript::RegisterFunction(char* FuncName , void* Func)
	{
		if (! m_LuaState)
			return false;
		lua_register(m_LuaState, FuncName, (lua_CFunction)Func);
		return true;
	}

	bool CLuaScript::Compile(char *)
	{
		return true;
	}

	// 功能:	批量注册Lua的内部C函数，各个函数的信息保存在reg_luafun的数据中
	// 参数:	reg_luafun *Funcs 数组的指针
	// 参数:	int n 函数数量。可以为零，由系统计算得到。
	bool CLuaScript::RegisterFunctions(reg_luafun Funcs[], int n)
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
// 		lua_baselibopen(m_LuaState);//Lua基本库
// 		Lua_OpenIOLib(m_LuaState);//输入输出库
// 		Lua_OpenStrLib(m_LuaState);//字符串处理库
// 		Lua_OpenMathLib(m_LuaState);//数值运算库
// 		//Lua_OpenDBLib(m_LuaState);//调试库
		luaL_openlibs( m_LuaState );
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

	//---------------------------------------------------------------------------
	// 函数:	CLuaScript::SafeCallBegin
	// SafeCallBegin与SafeCallEnd两函数应搭配使用，以防止在调用Lua的外部函数之后，
	//有多余数据在堆栈中未被清除。达到调用前与调用后堆栈的占用大小不变。
	//上述情况只需用在调用外部函数时，内部函数不需如此处理。
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
		if ((! m_IsRuning) && (m_LuaState))
		{
			m_IsRuning = true;
			return true;
		}
		return false;
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

	void CLuaScript::SetGlobalName(char* szName)
	{
		if (!szName) return ;
		lua_setglobal(m_LuaState, szName);
	}

	size_t CLuaScript::ModifyTable(char* szTableName) 
	{
		if (! szTableName[0])		return -1;

		int nIndex = lua_gettop(m_LuaState);

		lua_gettable(m_LuaState, szTableName);

		if (lua_gettop(m_LuaState) != ++nIndex)		return -1;

		return nIndex;
	}

}