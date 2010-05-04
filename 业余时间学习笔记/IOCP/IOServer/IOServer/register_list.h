#pragma once
#include "locks.h"
#include <list>
using namespace std;

class register_list
{

public:
	struct item_type
	{
		SOCKET sock;
		char   pcname[100];
		char   pcmac[20];

		item_type * remote;
		item_type( SOCKET , char * , char * , item_type *)
		{ }
	};

public:
	register_list(void);
	virtual ~register_list(void);
	
	/// 
	/// 
	/// 
public:
	void push_back( SOCKET sock )
	{
		sync::scope_guard sg( lock_item_ );
		item_type item ( sock ,NULL , NULL, NULL);
		items_.push_back( item );

		std::list<item_type>::iterator it = items_.begin() , end =items_.end();

		for ( ; it != end ; ++ it)
		{
			if ( it->sock == sock )
			{
				it->sock = sock;
				return ;
			}
		}
	}

	void push_back( SOCKET sock , const char * pcname_ , const char * pcmac_)
	{
		sync::scope_guard sg( lock_item_ );

		std::list<item_type>::iterator it = items_.begin() , end = items_.end();

		for (; it != end; ++it)
		{
			if ( strcmp(it->pcmac,pcmac_) == 0 || it->sock == sock )
			{
				it->sock = sock;
				strcpy_s( it->pcname , pcname_);
				strcpy_s( it->pcmac  , pcmac_ );

				return;
			}
		}

		item_type item (sock , "" , "" , 0 );
		strcpy_s(item.pcmac , pcmac_);
		strcpy_s(item.pcname , pcname_);
		items_.push_back( item );
	}


	void erase( SOCKET s)
	{
		sync::scope_guard  sg( lock_item_ );

		std::list<item_type>::iterator it = items_.begin() , end = items_.end();

		for ( ; it != end; ++it )
		{
			if ( it->sock == s)
			{
				items_.erase( it );
				break;
			}
		}
	}


	bool find( const char * pcmac_ , item_type & item )
	{
		sync::scope_guard sg( lock_item_ );
		
		std::list<item_type>::iterator it = items_.begin() , end = items_.end();

		for ( ; it != end; ++it )
		{
			if ( strcmp(it->pcmac,pcmac_) == 0 )
			{
				item = *it;

				return true;
			}
		}
		return false;
	}

	item_type* find(const char* mac_addr)
	{
		sync::scope_guard sg(lock_item_);

		std::list<item_type>::iterator it = items_.begin(), end = items_.end();
		for(; it != end; ++it)
		{
			if(  strcmp(it->pcmac , mac_addr) == 0)
				return &(*it);
		}

		return 0;		
	}

	item_type* find(SOCKET s)
	{
		sync::scope_guard sg(lock_item_);

		std::list<item_type>::iterator it = items_.begin(), end = items_.end();
		for(; it != end; ++it)
		{
			if(it->sock == s)
				return &(*it);
		}

		return 0;		
	}

	unsigned size() const
	{
		sync::scope_guard sg(lock_item_);

		return static_cast<unsigned>(items_.size());
	}

	std::list<item_type>  GetItems()const 
	{
		return items_;
	}

	/// 
	///  к╫сп╣д
	/// 
private:

public:
	mutable  sync::csectionlock lock_item_;
	std::list<item_type>  items_;

};
