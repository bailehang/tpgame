
#pragma  once


class BaseArg
{
public:
	BaseArg();
	virtual ~BaseArg();

	virtual void pushvalue( lua_State * L) = 0;
};

class IntArg : public BaseArg
{
private:
	int  m_value;

public:
	IntArg( int value ) : m_value ( value ) 
	{	}

	virtual void pushvalue ( lua_State * L )
	{
		lua_pushinteger( L , m_value );
	}
};


class BoolArg : public  BaseArg 
{
private :
	bool  m_value;

public:
	BoolArg( bool value ) : m_value ( value )
	{ }

	virtual void pushvalue( lua_State * L )
	{
		lua_pushboolean( L , m_value );
	}
};

class  FloatArg : public  BaseArg 
{
private:
	float  m_value;

public:
	FloatArg( float value ) : m_value ( value )
	{ }

	virtual  void pushvalue( lua_State * L )
	{
		lua_pushnumber(  L , m_value );
	}
};

class  DoubleArg : public BaseArg
{
private:
	double m_value;

public:

	DoubleArg ( double value ) : m_value ( value )
	{}

	virtual  void pushvalue( lua_State * L )
	{
		lua_pushnumber(  L , m_value );
	}
}

class  StringArg : public  BaseArg
{
private:
	std::string  m_value; 

public:
	StringArg( std::string& str) : m_value( str )
	{}

	virtual  void pushvalue( lua_State * L )
	{
		lua_pushstring(  L , m_value );
	}
};


class  ArgPool  
{
public:
	
	void push( int value )
	{
		BaseArg *arg = new  IntArg( value );
		m_vec.push_back( arg );		
	}

	void push(float value)
	{
		FloatArg *arg = new FloatArg( value );
		m_vec.push_back( arg );
	}

	void push(double value)
	{
		DoubleArg *arg = new DoubleArg( value );
		m_vec.push_back( arg );
	}

	void push(bool value)
	{
		BoolArg  *arg = new BoolArg( value );
		m_vec.push_back( arg );
	}

	void push(std::string& value)
	{
	    StringArg *arg = new StringArg( value );
		m_vec.push_back( arg );
	}

	int  pushvalue( lua_State * L  )
	{
		//m
		std::vector < BaseArg* >::iterator itr = m_vec.begin();
		
		for( ; itr != m_vec.end() ; itr ++ )
		{
			  (*itr)->pushvalue( L );
		}	
		return	 0;
	}	

private:
	std::vector < BaseArg* >  m_vec;
};


class CallLua
{	 
public:

	void  Call( ArgPool *Pool , std::string& funname )
	{
		lua_State* L = luaL_newstate();

		lua_getglobal( L , funname );
		
		if( lua_isfunction( L , -1) )
		{
			if(  0 == lua_pcall( L , Pool->pushvalue(L) , LUA_MULTRET , 0  ) )
				return 0;

			fprintf(stderr, "%s:%d call function failed:%s\n", __FILE__, __LINE__, luaL_checkstring(L, -1));   
		}

		return -1;
	}

};