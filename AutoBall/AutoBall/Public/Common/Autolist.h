/**
 *  brief  about an autolist
 *  file   Autolist.h
 *  date   2010/06/10
 */

#pragma  once

#include <list>

template < class T>
class   CAutoList
{
	typedef  std::list<T*>  ObjectList;

public:
	
	CAutoList()
	{
		m_Members.push_back( static_cast<T*>(this) );
	}

	~CAutoList()
	{
		m_Members.remove( static_cast<T*>(this) );
	}

	static  ObjectList&  GetAllMembers()  { return m_Members; }

private:

	static   ObjectList   m_Members;

};

template < class  T >
std::list<T*>  CAutoList<T>::m_Members;