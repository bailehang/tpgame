
#pragma  once

#include "lua_state.h"

template< typename T >
struct  typetype
{
	typedef T U;
};

inline  bool  Match( typetype<bool> , lua_State* L , int inx )
{
	return  lua_type( L , inx ) == LUA_TBOOLEAN;
}

inline  bool  Match( typetype<int> , lua_State* L , int inx )
{
	return  lua_type( L , inx ) == LUA_TNUMBER;
}

inline  bool  Match( typetype<const char*> , lua_State* L , int inx )
{
	return  lua_type( L , inx ) == LUA_TSTRING;
}

inline	bool  Get( typetype<bool> , lua_State* L , int idx )
{
	return	static_cast<bool>(lua_toboolean( L , idx ));
}

inline	int  Get( typetype<int> , lua_State* L , int idx )
{
	return	static_cast<int>(lua_tonumber( L , idx ));
}

inline	const char*  Get( typetype<const char*> , lua_State* L , int idx )
{
	return	lua_tostring( L , idx );
}

inline	void  Push( lua_State* L , int val )
{
	lua_pushnumber( L , val );
}

inline	void  Push( lua_State* L , bool val )
{
	lua_pushboolean( L , (int)val );
}

inline	void  Push( lua_State* L , const char* val )
{
	lua_pushstring( L , val );
}