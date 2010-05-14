
#pragma once

#include <lua.hpp>


static const luaL_reg lualibs[] = 
{
	{"base", luaopen_base},
	{"table", luaopen_table},
	{"io", luaopen_io},
	{"string", luaopen_string},
	{"math", luaopen_math},
	{"debug", luaopen_debug},
	{"loadlib", luaopen_package},
	/* add your libraries here */
	{NULL, NULL}
};


class state
{
public:
	state( bool  isOpen = false )
	{
		L = lua_open();
		if ( isOpen )
			luaL_openlibs( L );
	}

	~state()
	{
			lua_close( L );
	}

	lua_State * Lua_handle()
	{
		return L;
	}

	void open_stdlib()
	{
		const luaL_reg*  lib = lualibs;
		for ( ; lib->func ; lib++)
		{
			lib->func( L );
			lua_settop( L , 0 );
		}
	}

private:
	lua_State  *L;
};


/**
 *		static  state * GetLua()
 *	    { static  state*  st =  new state(true);  return  st;  }
 */