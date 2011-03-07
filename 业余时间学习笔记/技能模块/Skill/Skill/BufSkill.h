
#pragma  once 

eventtype
{

};

struct skillparam
{
	long  skillid;
	long  level;
	
	//执行脚本变量列表
	//CVariableList *pVariableList;
	//CMoveShape	*pFire;
	//CMoveShape	*pDest;
	//CMoveShape	*pUser;
	float			fMouseX;
	float			fMouseY;

	std::map<eventtype,
};

class  BufSkill
{
public:
	BufSkill();

	void  begin()
	{
		//m_machine->Enter();
		/// runsrcipt( scripts/skill/skillid.lua );
	}
	void  timeout()
	{
		m_machine->Execute();
	}
	void  proc(eventtype type)
	{
		std::map<eventtype,void*>::iterator itr = mapevent.find( type );

		if( itr != mapevent.end() ) m_machine->OnEvent( itr->second );
	}
	void  end()
	{
		m_machine->Exit();
	}

private:

	skillmachine<BufSkill>		*m_machine;
	std::map<eventtype,void*>	mapevent;
	skillparam					m_param;
};