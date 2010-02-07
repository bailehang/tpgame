
/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/4]
*/ 

#pragma  once
#include <map>

namespace tp_script
{

	struct tableitem 
	{
		int   m_tid;
		int   m_states;
		int   m_type;
		void  *m_pPtr;
	};

	enum eitemtype
	{
		EInValid  =  -1 ,
		ENum ,
		EString,
		EFun,
	};
	

	class  TableList
	{
		typedef std::map<std::string, tableitem > TableL;
		typedef TableL::iterator                  TIter;

		enum 
		{
			ID_EMPTY  = 0 ,
			ID_USE	 ,
			ID_SET   ,
		};

	public:

		TableList() ;

		~TableList();

		template< typename Var>
		bool   AddTable(const char* name , const Var & value )
		{
			//tableitem  item.m_pPtr = value;
			//m_tablelist[ name ] = item;
		}
		
		template< typename Var>
		Var&  GetTable( const char * name )
		{
			TIter  iter = m_tablelist.find( name );
			if ( iter != m_tablelist.end() )
			{
				return iter->second;
			}
			return NULL;
		}

		void   RemoveTable(const char* name )
		{
			TIter  iter = m_tablelist.find( name );
			if ( iter != m_tablelist.end() )
			{
				m_tablelist.erase( iter );
			}
		}

		void   Cleanup()
		{
			m_tablelist.clear();
		}


	private:
		TableL       m_tablelist;

	};

}