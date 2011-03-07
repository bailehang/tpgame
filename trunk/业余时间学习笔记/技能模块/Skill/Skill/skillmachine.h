
/**
 * http://blog.csdn.net/firebird321/archive/2008/05/19/2458455.aspx
 */	   
template <typename T>
class  skillmachine
{
	typedef  T  type;

public:

	void  Enter()
	{
		lua_State *L;
		L = luaL_newstate();

	}

	void  Exit();

	void  Execute()
	{
		//lual_pcall( funname , skillparam );
	};

	void  OnEvent();

private:

	type		*m_type;

	lua_State  * L;
	lua_State   *m_luastate;
	std::string  m_bname;
	std::string  m_timeout;
	std::string  m_end;
	
};