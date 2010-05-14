
#pragma  once

#include <map>
#include <list>
using namespace std;

template < typename ID, typename Class,typename AlloFun , typename Rel>
class  Factory
{
	struct  FWapper
	{
		FWapper(AlloFun a,Rel r,class wp):crtor(a),ret(r),Wapper(wp)
		{
		}
		AlloFun  crtor;
		Rel		 ret;
		Class	 Wapper;
	};

	typedef std::map<ID,Class> CallMap;

public:
	Factory( AlloFun fun, Rel rel )
	{
		Clear();
 		m_Alloc = fun;
 		m_Rel   = rel;
	}

	~Factory()
	{
		Clear();
	}
	
	void Clear()
	{
		CallMap::iterator  itr = m_MapList.begin();
		for( ; itr != m_MapList.end() ; itr ++ )
		{
			delete itr->second;
		}
		m_MapList.clear();
	}

	void  Add(ID id,Class cl)
	{
		m_MapList.insert( std::pair<ID,Class>(id,cl) );
	}

	void* Alloc( ID id )
	{
		CallMap::iterator  itr = m_MapList.find( id );
		if ( itr != m_MapList.end() )
		{
			return  (itr->second->*m_Alloc)();
		}
		return NULL;
	}

	bool   Release(ID id , void* p )
	{
		CallMap::iterator  itr = m_MapList.find( id );
		if ( itr != m_MapList.end() )
		{
			return  (itr->second->*m_Rel)(p);
		}
		return false;
	}

private:
	CallMap   m_MapList;
	AlloFun	  m_Alloc;
	Rel		  m_Rel;
};