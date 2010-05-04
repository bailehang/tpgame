#include "stdafx.h"
#include "locks.h"

namespace sync
{
	super_lock::~super_lock(){}

	scope_guard::scope_guard(super_lock& lock)
		:lock_(lock)
	{
		lock.enter();
	}

	scope_guard::~scope_guard()
	{
		lock_.leave();
	}
	
	eventlock::eventlock()
		:handle_(0)
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
		return ((handle_ = ::CreateEvent(0, 0, 1, 0)) != 0);
	}

	void eventlock::close()
	{
		if(handle_)
		{
			leave();
			::CloseHandle(handle_);
			handle_ = 0;
		}
	}

	bool eventlock::enter()
	{
		if(handle_)
			return (::WaitForSingleObject(handle_, INFINITE) != WAIT_TIMEOUT);

		return false;
	}

	void eventlock::leave()
	{
		if(handle_)
			::SetEvent(handle_);
	}

	csectionlock::csectionlock()
		:is_ok_(false)
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
		is_ok_ = true;

		return true;
	}

	void csectionlock::close()
	{
		if(is_ok_)
		{
			leave();
			::DeleteCriticalSection(&cs_);
		}
		is_ok_ = false;
	}

	bool csectionlock::enter()
	{
		if(is_ok_)
		{
			::EnterCriticalSection(&cs_);
			return true;
		}
		return false;
	}

	void csectionlock::leave()
	{
		if(is_ok_)
			::LeaveCriticalSection(&cs_);
	}

}
