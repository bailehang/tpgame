/**
 * File  :
 * Brief :
 * Author:  Expter  
 * Date  :  [2010-2-5]
 *                             
 **/

#pragma  once
#include "../UnitLua.h"
#include <iostream>
#include <string>
using namespace  std;

namespace  tp_script
{
	int   luatest1( lua_State* L)
	{
		int		tid ;
		double  dv;
		std::string str;

		//if (!Match(typetype<int>(),L,1))	return 0;
		
		tid = (int)luaL_checkinteger(L,1);

		//if (!Match(typetype<double>(),L,2)) return 0;
		
		dv= (double)luaL_checkinteger(L,2);

		//if (!Match(typetype<const char*>(),L,3)) return 0;

		str=luaL_checkstring(L,3);

		std::cout << "lua test1 " << tid << " " << dv <<" " << str.c_str() <<std::endl;

		return 0;
	}

	int   luatest2( lua_State* L)
	{
		int		tid ;
		double  dv;
		std::string str;

		//if (!Match(typetype<int>(),L,1))	return 0;

		tid = (int)luaL_checkinteger(L,1);

		//if (!Match(typetype<double>(),L,2)) return 0;

		dv= (double)luaL_checkinteger(L,2);

		//if (!Match(typetype<const char*>(),L,3)) return 0;

		str=luaL_checkstring(L,3);

		std::cout << "lua test2 " << tid << " " << dv <<" " << str.c_str() <<std::endl;

		return 0;
	}

	int   luatest3( lua_State* L)
	{
		lua_getglobal(L , "test3" );

		Push(L , 10 );
		Push(L , true);
		Push(L , "xxx");

		return 3;
	}

}
