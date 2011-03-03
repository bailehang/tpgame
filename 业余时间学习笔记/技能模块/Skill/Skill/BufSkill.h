
#pragma  once 

eventtype
{

};

struct skillparam
{

};

class  BufSkill
{
public:
	BufSkill();

	void  begin();
	void  timeout();
	void  proc();
	void  end();

private:

	skillmachine<BufSkill>		*m_machine;
	std::map<eventtype,void*>	mapevent;
	skillparam					m_param;
};