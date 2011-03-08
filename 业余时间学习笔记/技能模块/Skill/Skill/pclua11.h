

#pragma  once 

#include <iostream>

template < typename t1 , typename  t2 >
class  Duo
{

};

class  NullT
{

};


class  LuaCallBase
{
public:

	LuaCallBase( lua_State * L , const std::string &funtionname)
	{
		m_lua = L;		
	}

protected:
	
	void   push( const int & value )
	{
		lua_pushinteger( m_lua , value );
	}

	void   push( const float& value )
	{
		lua_pushnumber( m_lua , value );
	}

	void   push( const double& value )
	{
		lua_pushnumber( m_lua , value );
	}

	void   push( const std::string& value)
	{
		lua_pushnumber( m_lua , value );
	}

	int		get() const{ return lua_tointeger( L , -1); }
	float	get() const{ return lua_tonumber(L , -1); }
	double  get() const{ return lua_tonumber(L , -1); }
	std::string get() const { return lua_tostring( L , -1 ); }

	NullT   get() const{ return NullT(); }


protected:
	lua_State   * m_lua;
};

template < typename TR , 
		   typename T1 = NullT ,
		   typename T2 = NullT ,
		   typename T3 = NullT ,
		   typename T4 = NullT ,
>

class  LuaCall 
	: public  Duo < TR , typename LuaCall< T1,T2,T3,T4,NullT > >
	: public  LuaCallBase
{

public:
	LuaCall( lua_State * L , std::string& funname ) : LuaCallBase( L , funname )
	{	   
		lua_getglobal( L ,  funtionname.c_str );
	}

	TR Call( T1 t1 , T2 t2 , T3 t3 , T4 t4)
	{
		TR  ret ;
		push( t1 );
		push( t2 );
		push( t3 );
		push( t4 );

		if ( lua_pcall( L , 4 , 1 , 0 ) != 0 )
		{
			std::cout << "run lua function error " << std::endl;
		}

		ret = get();

		return ret ;
	}
};

template < typename TR , 
		   typename T1 ,
		   typename T2 ,
		   typename T3 
>
class  LuaCall < TR , T1 , T2 , T3 , NullT >
	: public Duo < TR , T1 >
	: public LuaCallBase
{
public:

	LuaCall( lua_State * L , std::string & funname ) : LuaCallBase ( L , funname )
	{					 
	}

	TR Call ( T1 t1 , T2 t2 , T3 t3)
	{
		TR ret ;
		push( t1 );
		push( t2 );
		push( t3 );

		if ( lua_pcall( L , 3 , 1 , 0) != 0 )
		{
			std::cout << " fun Luaction error " << std::endl;
		}

		ret = get();

		return ret;
	}
}


template < typename TR , 
		   typename T1 ,
		   typename T2 
>
class  LuaCall < TR , T1 , T2 , NullT , NullT >
	: public Duo < TR , T1 >
	: public LuaCallBase
{
public:

	LuaCall( lua_State * L , std::string & funname ) : LuaCallBase ( L , funname )
	{
	}

	TR Call ( T1 t1 , T2 t2 )
	{
		TR ret ;
		push( t1 );
		push( t2 );

		if ( lua_pcall( L , 2 , 1 , 0) != 0 )
		{
			std::cout << " fun Luaction error " << std::endl;
		}

		ret = get();

		return ret;
	}
}

template < typename TR , 
		   typename T1 
>
class  LuaCall < TR , T1 , NullT , NullT , NullT >
	: public Duo < TR , T1 >
	: public LuaCallBase
{
public:

	LuaCall( lua_State * L , std::string & funname ) : LuaCallBase ( L , funname )
	{
	}

	TR Call ( T1 t1  )
	{
		TR ret ;
		push( t1 );

		if ( lua_pcall( L , 1 , 1 , 0) != 0 )
		{
			std::cout << " fun Luaction error " << std::endl;
		}

		ret = get();

		return ret;
	}
}

template < typename TR >
class  LuaCall < TR , NullT , NullT , NullT , NullT >
	: public Duo < TR , NullT >
	: public LuaCallBase
{
public:

	LuaCall( lua_State * L , std::string & funname ) : LuaCallBase ( L , funname )
	{
	}

	TR Call (  )
	{
		TR ret ;
		if ( lua_pcall( L , 0 , 1 , 0) != 0 )
		{
			std::cout << " fun Luaction error " << std::endl;
		}

		ret = get();

		return ret;
	}
}

template <  >
class  LuaCall < NullT , NullT , NullT , NullT , NullT >
	: public Duo < NullT , NullT >
	: public LuaCallBase
{
public:

	LuaCall( lua_State * L , std::string & funname ) : LuaCallBase ( L , funname )
	{
	}

	void Call (  )
	{
		if ( lua_pcall( L , 0 , 0 , 0) != 0 )
		{
			std::cout << " fun Luaction error " << std::endl;
		}
	}
}
		   