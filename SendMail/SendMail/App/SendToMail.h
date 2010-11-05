

#pragma  once 

#include "../Public/BaseDef.h"

class  SendToMail
{
public:
	SendToMail(size_t size )
	{
		m_list.clear();
		m_SendID = size;
	}

	void  AppendUser(std::list<std::string>& List)
	{
		m_list = List;
	}			 
	bool  operator() ( )
	{
		return Send( m_SendID );
	}

private:
	bool  Send( size_t index );

public:

	/// 联系ID 
	size_t		    m_SendID;
	/// 发送联系人
	std::list<std::string>  m_list;

};