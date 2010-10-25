#pragma once

#include "../public/locks.h"

/// 线程池
namespace tp_ipc_peer_namespace
{
	/// 接口
	class task_object
	{
	public:
		virtual ~task_object(){}
		virtual void exec() = 0;
	};

	template< typename Functor>
	class task 
		: public task_object
	{

		/// 禁止操作
	private:
		task( const task &);
		task & operator =( const task & );

	public:
		typedef Functor functor_type;

		task( const functor_type & functor)
			: functor_( functor )
		{ }

		/// 执行
		virtual void exec()
		{
			functor_();
		}

	private:
		Functor functor_;
		
	};

	class ctpool
	{
		typedef ctpool self_type;
		
	public:
		ctpool(void)
			:tpool_running_( true )
		{ 
			_m_start_threads( 1 );
		}
		ctpool ( unsigned threadsize )
			:tpool_running_(true)
		{
			_m_start_threads( threadsize );
		}

		template< typename Function>
		void push( const Function & f)
		{
			/// 枷锁
			task_lock_.enter();

			task_container_.push_back( new tp_ipc_peer_namespace::task<Function>( f ) );
			
			task_lock_.leave();

		}
		
		~ctpool(void){}

	private:

		/// 创建线程池
		void _m_start_threads( unsigned size )
		{
			if ( size == 0 )
				size = 4;

			for ( unsigned i = 0 ; i < size ; i++)
			{
				tinfo_type tinfo;
				tinfo.state = 0;
				tinfo.handle = (HANDLE)::_beginthreadex( 0 , 0 , _m_work_thread , NULL , NULL ,&(tinfo.tid) );
				threads_.push_back(  tinfo );
			}
		}
		
		/// 唤醒
		void _m_wakeup()
		{
			HANDLE handle = 0;

			/// 对共享区 枷锁
			tlock_.enter();
			std::vector<tinfo_type>::iterator it = threads_.begin(), end = threads_.end();

			for ( ; it != end ; ++it )
			{
				if ( it->state == 0 ) /// 在等待状态
				{
					handle	  =  it->handle ;
					it->state = 1;
					break;
				}
			}
			tlock_.leave();

			while ( ::ResumeThread( handle ) != 1)
				;
		}

		/// 挂起某个线程
		void _m_suspend()
		{
			unsigned tid = ::GetCurrentThreadId();
			HANDLE   handle = 0;

			tlock_.enter();

			/// 对共享区 枷锁
			tlock_.enter();
			std::vector<tinfo_type>::iterator it = threads_.begin(), end = threads_.end();

			for ( ; it != end ; ++it )
			{
				if ( it->tid == tid ) /// 运行ID
				{
					handle	  =  it->handle ;
					it->state = 0;
					break;
				}
			}
			tlock_.leave();

			/// 挂起
			if ( handle)
			{
				::SuspendThread( handle );
			}
		}

		/// 获取task
		tp_ipc_peer_namespace::task_object * _m_read_task()
		{
			while( tpool_running_ )
			{
				tp_ipc_peer_namespace::task_object * task = NULL;
				
				/// 对共享区 枷锁
				task_lock_.enter();
				if ( task_container_.size() )
				{
					task = *( task_container_.begin() );
					task_container_.erase( task_container_.begin() );
				}
				task_lock_.leave();

				if ( task )
				{
					return task;
				}
				else
					_m_suspend();
			}
			return NULL;
		}

	private:
		static  unsigned __stdcall _m_work_thread(void * arg)
		{
			
			self_type & self = *reinterpret_cast<self_type*>(arg);
			tp_ipc_peer_namespace::task_object * task = 0;

			::SuspendThread(::GetCurrentThread());

			while( true )
			{
				task = self._m_read_task();
				if ( task )
				{
					task->exec();

					delete task ;
					task = 0;
				}
				else
					break;
			}
			
			::_endthreadex( 0 );
			return 0;
		}

	private:
		/// 线程信息
		struct tinfo_type
		{
			HANDLE			handle;
			unsigned		tid;
			unsigned long	state;  // 0 = sleep;
		};

		/// user define var
	private:
		/// 线程运行状态
		volatile bool				tpool_running_;
		/// 一个临界区类
		sync::csectionlock			tlock_;                
		/// 线程信息
		std::vector<tinfo_type>		threads_;		
		/// 
		sync::csectionlock			task_lock_;
		/// 一个回调函数
		std::vector<task_object* > task_container_;

	};


}