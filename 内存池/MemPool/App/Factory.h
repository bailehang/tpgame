/** 
 *   @brief 工厂类的实现
 *
 */

#pragma  once 

#include <map>

template < typename type ,
		   typename Alloctor,
		   typename AllocFun,
		   typename DestoryFun>
class  Factory
{
	typedef Factory		self_type;
	typedef AllocFun	alloc_fun;
	typedef DestoryFun  destory_fun;
	typedef type		value_type;
	typedef Alloctor    alloc_type;
	typedef std::map<type,alloc_type> alloctable;

public:
	Factory( alloc_fun alloc , destory_fun destory )
	{
		m_alloc = alloc;
		m_destory = destory;
	}

	~Factory()
	{
		Clear();
	}

	void  Clear()
	{
		alloctable::iterator it = m_allocmap.begin();

		for( ; it != m_allocmap.end() ; it ++ )
		{
			delete it->second;
		}
		m_allocmap.clear();
	}

	void  Register( value_type index, alloc_type atype)
	{
		m_allocmap.insert( std::pair<value_type,alloc_type>( index , atype) );
	}

	void* Alloc(value_type index)
	{
		alloctable::iterator it = m_allocmap.find( index );

		if( it != m_allocmap.end() )
			return (it->second->*m_alloc)();

		return NULL;
	}

	bool  Free(value_type  index,void *pData)
	{
		alloctable::iterator it = m_allocmap.find( index );

		if( it != m_allocmap.end() )
			return (it->second->*m_destory)(pData);

		return false;
	}

	bool  CheckExist( value_type index ) const 
	{
		alloctable::const_iterator it = m_allocmap.find( index );

		if( it != m_allocmap.end() )
			return true;

		return false;
	}

private:

	alloctable		m_allocmap;
	alloc_fun		m_alloc;
	destory_fun		m_destory;

};