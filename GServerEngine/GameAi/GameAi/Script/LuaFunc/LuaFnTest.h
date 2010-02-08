/**
 * File  :
 * Brief :
 * Author:  Expter  
 * Date  :  [2010-2-5]
 *                             
 **/

#pragma  once
#include "../Lua/lua.hpp"
#include <iostream>
#include <string>
using namespace  std;

namespace  tp_script
{
	int   luatest1( lua_State* L)
	{
		int  tid = (int)lua_tonumber(L,-1);
		double dv= (double)lua_tonumber(L,-2);

		std::string str=lua_tostring(L,-3);

		std::cout << "lua test1 " << tid << " " << dv <<" " << str.c_str() <<std::endl;

		return 0;
	}

	int   luatest2( lua_State* L)
	{
		int  tid = (int)lua_tonumber(L,-1);
		double dv= lua_tonumber(L,-2);

		std::string str=lua_tostring(L,-3);

		std::cout << "lua test1 " << tid << " " << dv <<" " << str.c_str() <<std::endl;

		return 0;
	}
}
