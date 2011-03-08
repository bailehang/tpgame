
/**
 * http://blog.csdn.net/firebird321/archive/2008/05/19/2458455.aspx
 */	   
template <typename T>

#include "pcall.h"

template < class T >
class  skillmachine
{
	typedef  T  type;

public:

	void  Enter()
	{
		long id =  static_cast< BufSkill* > ( m_type )->getId();

		char str[100];
		
		fprintf( str , "script/skill/%d.lua" , id); 

		RunScript( str );
		
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