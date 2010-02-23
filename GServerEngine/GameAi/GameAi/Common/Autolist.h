/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/23]
*/

#pragma  once

#include <list>

template < class vclass>
class   CAutoList
{
	typedef  std::list<vclass*>  ObjectList;

public:
	
	CAutoList()
	{
		m_Members.push_back( static_cast<vclass*>(this) );
	}

	~CAutoList()
	{
		m_Members.remove( static_cast<vclass*>(this) );
	}

	static  ObjectList&  GetAllMembers() cosnt { return m_Members; }

private:

	static   ObjectList   m_Members;

};

template < class  vclass >
std::list<vclass*>  CAutoList<vclass>::m_Members;