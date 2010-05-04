#include "StdAfx.h"
#include "Lock.h"

namespace sync
{
	Super_lock::Super_lock(){}
	Super_lock::~Super_lock(){}

	scope_guard::scope_guard( Super_lock & _lock) : lock(_lock)
	{
		lock.enter();
	}
	scope_guard::~scope_guard()
	{
		lock.leave();
	}

	eventlock::eventlock()
		:handle_( 0 )
	{
		open();
	}
	eventlock::~eventlock()
	{
		close();
	}

	bool eventlock::open()
	{
		close();
		return ( ( handle_ = ::CreateEvent(0,0,1,0) ) != 0 );
	}

	bool eventlock::enter()
	{
		state_ = state_type::none;
		if ( handle_ )
		{
			if ( ::WaitForSingleObject( handle_,timeout_) != WAIT_TIMEOUT)
			{
				return true;
			}
			else
				state_ = state_type::timeout;
		}

		return false;
	}

	void eventlock::leave()
	{
		if ( handle_ )
			::SetEvent( handle_ );
	}
	
	void eventlock::close()
	{
		if ( handle_ )
		{
			leave();
			::CloseHandle( handle_ );
			handle_ = 0;
		}
	}

	csectionlock::csectionlock()
		:is_ok(false)
	{
		open();
	}

	csectionlock::~csectionlock()
	{
		close();
	}

	bool csectionlock::open()
	{
		close();
		::InitializeCriticalSection(&cs_);
		is_ok= true;

		return true;
	}

	void csectionlock::close()
	{
		if(is_ok)
		{
			leave();
			::DeleteCriticalSection(&cs_);
		}
		is_ok = false;
	}

	bool csectionlock::enter()
	{
		if(is_ok)
		{
			::EnterCriticalSection(&cs_);
			return true;
		}
		return false;
	}

	void csectionlock::leave()

	{
		if(is_ok)
			::LeaveCriticalSection(&cs_);
	}


}