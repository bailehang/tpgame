#ifndef LOCKS_HPP
#define LOCKS_HPP

#include <windows.h>

namespace sync
{
	class super_lock
	{
	public:
		virtual ~super_lock();

		virtual bool open() = 0;
		virtual void close() = 0;
		virtual bool enter() = 0;
		virtual void leave() = 0;
	};

	class scope_guard
	{
	public:
		scope_guard(super_lock& lock);
		~scope_guard();
	private:
		super_lock& lock_;
	};

	class eventlock
		:public super_lock
	{
	public:
		eventlock();
		~eventlock();
		bool open();
		void close();
		bool enter();
		void leave();
	private:
		HANDLE handle_;

	};

	class csectionlock
		:public super_lock
	{
	public:
		csectionlock();
		~csectionlock();
		bool open();
		void close();
		bool enter();
		void leave();
	private:
		bool				is_ok_;
		::CRITICAL_SECTION	cs_;
	};


}//end namespace sync

#endif