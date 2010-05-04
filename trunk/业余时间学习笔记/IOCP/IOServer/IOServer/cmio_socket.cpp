#include "StdAfx.h"
#include "cmio_socket.h"


namespace _tp_io_complete_portname
{

	std::vector<callback_info>	cmio_socket::callbacks;
	std::set<SOCKET>		cmio_socket::socket_set_;
	cmio_socket::cmio_socket(void)
		:listener_socket_( values::null_socket) ,iocp_(INVALID_HANDLE_VALUE), threads_prepared( false )
	{
		_m_make_complete_io_port();
	}

	cmio_socket::~cmio_socket(void)
	{
		///  释放资源
		_m_close_complete_io_port();
		_m_close_socket();
	}

	
	void cmio_socket::_m_make_complete_io_port()
	{
		/// 先关闭完成端口
		_m_close_complete_io_port();

		iocp_ = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE , 0 , 0 , 0 );

		_m_make_threads();
	}

	void cmio_socket::_m_close_complete_io_port()
	{
		if ( iocp_ != INVALID_HANDLE_VALUE)
		{
			::CloseHandle( iocp_ );
			iocp_ = INVALID_HANDLE_VALUE;
		}
	}

	void cmio_socket::_m_open_socket()
	{
		_m_close_socket();

		WSADATA wsa_data;
		::WSAStartup(MAKEWORD( 2, 2 ), &wsa_data);

		listener_socket_ = ::WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
	}

	///
	///  关闭socket
	///
	void cmio_socket::_m_close_socket()
	{
		if ( listener_socket_ != values::null_socket)
		{
			::closesocket( listener_socket_ );
			listener_socket_ = values::null_socket;
			::WSACleanup();
		}
	}

	///
	/// 开启工作线程池
	///
	void cmio_socket::_m_make_threads()
	{
		if ( threads_prepared == false )
		{
			SYSTEM_INFO info;
			::GetSystemInfo( &info);

			int cpusize = info.dwNumberOfProcessors * 2 -2  ;

			for( int size = 0 ; size < cpusize ; size ++)
			{
				unsigned tid ;
				HANDLE handle_thread = (HANDLE)::_beginthreadex( 0 , 0  , _m_work_thread , this , 0 , &tid );

				/// 可以记录每个线程的信息
				/// id , thread_handle , thread_id
			}

			threads_prepared = true;
		}
	}

	/// 接受消息
	void cmio_socket::_m_recv( SOCKET s , operation_data * opdata )
	{
		//opdata->bytes_recv = BUFFERSIZE;
		opdata->wsabuf.len = BUFFERSIZE;
		opdata->wsabuf.buf = opdata->data;

		//static  long sum =  0;
		//sum ++;
		if ( strcmp( opdata->data, "end") == 0)
		{
			opdata->print();

			//std::cout <<" sum = " << sum << std::endl;
		}
		

		unsigned long bytes_recv;
		unsigned long flags = 0;

		int x = ::WSARecv( s , &(opdata->wsabuf) , 1 , &bytes_recv, & flags , &(opdata->overlapped) , 0 );

	}
	
	/// 回调函数处理
	void cmio_socket::_m_call_handler(int type, SOCKET s, void *socket_extra, const char *buf, unsigned long bufsize)
	{
		callback_info  * cinfo =  NULL;

		size_t count = 0;
		sync::scope_guard sg( callback_lock_ );
		count = callbacks.size();

		if ( count )
		{
			cinfo = new callback_info[ count ];

			std::vector<callback_info>::iterator it = callbacks.begin() , end = callbacks.end();
		
			for ( int i = 0 ; it != end ; ++it )
			{
				cinfo[i++] = *it;
			}	
		}

		if ( cinfo )
		{
			for ( size_t i = 0 ; i < count ; ++i )
			   cinfo[i].handler( type ,s,socket_extra , buf , bufsize , cinfo[i].userdata );

			delete [] cinfo;
		}
	}

	void cmio_socket::handler( handler_funtion handle , void * userdata )
	{
		sync::scope_guard sg (callback_lock_); 

		callback_info  info= { handle , userdata };

		callbacks.push_back( info );

	}

	/// 监听
	bool cmio_socket::listener( int port ,  int connections )
	{
		_m_open_socket();

		if ( listener_socket_ == values::null_socket)
			return false;
		
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = ::htonl( INADDR_ANY );
		addr.sin_port   = ::htons(port);

		if ( ::bind( listener_socket_ , reinterpret_cast<sockaddr*>(&addr) , sizeof (addr)))
			return false;
		
		return ( ::listen(listener_socket_ , connections) == 0);
		
	}

	/// 接收，
	bool cmio_socket::accept(void *(*extra_maker)(SOCKET, void *), void *arg)
	{
		if ( listener_socket_ != values::null_socket)
		{
			SOCKET  acc_socket = ::WSAAccept( listener_socket_ , 0 , 0 , 0, 0);
			{
				sync::scope_guard sg ( socket_set_lock_);
				socket_set_.insert( acc_socket );
			}

			
			handler_data *ptr = new handler_data;
			ptr->sock  = acc_socket;

			if (extra_maker)
				ptr->extra = extra_maker( acc_socket , arg );
			
		
			::CreateIoCompletionPort( reinterpret_cast<HANDLE>(acc_socket),iocp_ , reinterpret_cast<ULONG_PTR>(ptr) , 4);

			operation_data *pdata = new operation_data;
			
			memset(&(pdata->overlapped),0,sizeof(pdata->overlapped));

			pdata->wsabuf.len = BUFFERSIZE;
			pdata->wsabuf.buf = pdata->data;

			unsigned long bytes_recv;
			unsigned long flags = 0;

			::WSARecv( acc_socket , &(pdata->wsabuf) , 1 , &bytes_recv, &flags , &(pdata->overlapped) , 0 );

			return true;	
		}
		return false;
	}
	unsigned __stdcall cmio_socket::_m_work_thread( void * arg)
	{
		self_type self = *reinterpret_cast<self_type*>(arg);

		HANDLE  iocp   = self.iocp_;
		unsigned long bytes_transferred ;
		handler_data * sockinfo= 0;
		operation_data *opdata = 0;

		unsigned long tid = ::GetCurrentThreadId();

		for ( ;; )
		{
			/************************************************************************/
			/* 参数1  Comp1etionPort   iocp的handle
			/* 参数2  lpNumber0fBytesTransferred 对于wasrecv/send  接收，发送实际传输的字节数
			/* 参数3  当文件I/O操作完成后，用于存放与之关联的CK。  socket , extrea
			/* 参数4  为调用IOCP机制所引用的OVERLAPPED结构。
			/* 参数5  超时时间
			/************************************************************************/
			if ( 0 == GetQueuedCompletionStatus( iocp ,&bytes_transferred, reinterpret_cast<LPDWORD>(&sockinfo) , reinterpret_cast<LPOVERLAPPED *>(&opdata) , INFINITE))
			{
				if(GetLastError() == WAIT_TIMEOUT || GetLastError() == ERROR_NETNAME_DELETED)
				{
					sync::scope_guard sg(self.socket_set_lock_);
					std::set<SOCKET>::iterator it = self.socket_set_.find( sockinfo->sock );
					if ( it != self.socket_set_.end() )
					{
						self._m_call_handler( 0 , sockinfo->sock , sockinfo->extra ,opdata->data , bytes_transferred);

						::closesocket( sockinfo->sock );

						delete opdata;
						self.socket_set_.erase( it );
					}
					continue;
				}
				return 0;
			}

			if ( bytes_transferred == 0 )
			{
				sync::scope_guard sg(self.socket_set_lock_);
				std::set<SOCKET>::iterator it = self.socket_set_.find( sockinfo->sock );

				if ( it != self.socket_set_.end() )
				{
					self._m_call_handler( 0 , sockinfo->sock , sockinfo->extra ,opdata->data , bytes_transferred);

					::closesocket( sockinfo->sock );

					delete opdata;
					self.socket_set_.erase( it );
				}
				continue;
			}

			//opdata->print();
			//std::cout << opdata->data << std::endl;
			static  long sum =  0;
			static  long total= 0;
			sum ++;
			total++;
			if ( strcmp( opdata->data, "end") == 0)
			{
				std::cout <<" sum = " << sum << std::endl;
				std::cout <<" total ="<< total << std::endl;
				sum = 0;
			}

			self._m_call_handler( 1 , sockinfo->sock , sockinfo->extra , opdata->data , bytes_transferred );

			_m_recv( sockinfo->sock , opdata );
			
		}

		_endthreadex( 0 );

		return 0;
	}
 
}