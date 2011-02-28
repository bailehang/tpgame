
#pragma  once

#include "eEventDef.h"
#include <map>
#include <list>
using namespace std;

template < typename Id, typename Type,typename Alloc, typename Destory>
class  Factory
{
	typedef Factory		 SelfObj;
	typedef Alloc		 AllocFun;
	typedef	Destory		 DestoryFun;
	typedef typename Id  FactoryId;
	typedef std::map<Id,Type> AllocMap;
public:
	Factory( AllocFun alloc,DestoryFun destory)
	{
		Clear();
 		m_AllocFun = alloc ;
		m_DestoryFun= destory;
	}

	~Factory()
	{
		Clear();
	}
	
	void Clear()
	{
		AllocMap::iterator  itr = m_MapAlloc.begin();
		for( ; itr != m_MapAlloc.end() ; itr ++ )
		{
			delete itr->second;
		}
		m_MapAlloc.clear();
	}

	void  Add(Id size , Type type )
	{
		m_MapAlloc.insert( std::pair<Id,Type>(size,type) );
	}

	void* Alloc( Id id )
	{
		AllocMap::iterator  itr = m_MapAlloc.find( id );
		if ( itr != m_MapAlloc.end() )
		{
			return  (itr->second->*m_AllocFun)();
		}
		return NULL;
	}

	bool   Release(Id id , void* p )
	{
		AllocMap::iterator  itr = m_MapAlloc.find( id );
		if ( itr != m_MapAlloc.end() )
		{
			return  (itr->second->*m_DestoryFun)( p );
		}
		return false;
	}

	bool	CheckExist( Id id)
	{
		AllocMap::iterator  itr = m_MapAlloc.find( id );
		if ( itr != m_MapAlloc.end() )
		{
			return true;
		}
		return false;
	}

	void  Print()
	{
		AllocMap::iterator  itr = m_MapAlloc.begin();
		for( ; itr != m_MapAlloc.end() ; itr ++ )
		{
			itr->second->Print();
		}
	}

private:
	AllocMap   m_MapAlloc;
	AllocFun   m_AllocFun;	
	DestoryFun m_DestoryFun;
};