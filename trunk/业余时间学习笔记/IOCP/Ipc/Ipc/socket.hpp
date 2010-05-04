#pragma  once

#include <winsock2.h>
#include "lock.h"

namespace netpack
{
	///
	/// 网络初始化
	/// 
	class wsa_life
	{
	public:
		wsa_life()
		{
			WSADATA wsa_data;
			::WSAStartup( MAKEWORD(2,2),&wsa_data);
		}
		~wsa_life()
		{
			::WSACleanup();
		}
	};


	/// 
	/// 网络通信函数封装
	///
	class socket_type
	{
		typedef socket_type self_type;
	public:
		typedef unsigned streamsize;
		
		/// 定义无连接的套接字默认值
		struct values
		{
			enum { null_socket = INVALID_SOCKET ,};
		};
		
		/// 构造函数
		socket_type()
			:handle_(values::null_socket)
		{}

		socket_type( SOCKET s, int address_family )
			:handle_(s), addr_family_(address_family)
		{}

		socket_type(int address_family, int protocol , bool is_stream)
			:handle_( values::null_socket)
		{
			open( address_family ,protocol , is_stream);
		}
		
		socket_type( const self_type & rhs)
			:handle_(rhs.handle_) , addr_family_( rhs.addr_family_)
		{
			rhs.handle_ = values::null_socket;
		}

		/// 重写操作符
		self_type & operator = ( const self_type & rhs)
		{
			if ( this != &rhs)
			{	
				handle_ = rhs.handle_;
				addr_family_ = rhs.addr_family_;
				rhs.handle_ = values::null_socket;
			}
			return *this;
		}

		~socket_type()
		{
			close();
		}

		///
		/// 逻辑函数
		///

		/// 是否连接
		bool empty() const{ return (handle_ == values::null_socket);}

		/// 得到socket
		SOCKET GetHandle() const { return handle_ ; }

		/// 初始化套接字
		bool open( int address_ , int proctorl , bool is_stream )
		{
			close();

			handle_ = ::socket( address_ , (is_stream ? SOCK_STREAM : SOCK_DGRAM ) , proctorl );
			
			addr_family_ = address_;

			return ( handle_ != values::null_socket );
		}

		/// 关闭套接字
		void close()
		{
			if ( values::null_socket != handle_)
			{
				::closesocket( handle_ );
				handle_ = values::null_socket;
			}
		}

		/// 连接函数
		bool connect ( const char * hostname , int port )
		{
			if( handle_ != values::null_socket)
			{
				struct sockaddr_in server;
				server.sin_family = addr_family_;
				server.sin_port   = htons(port);
				server.sin_addr.s_addr   = inet_addr(hostname);

				if ( server.sin_addr.s_addr == INADDR_NONE)
				{
					struct hostent * host = ::gethostbyname( hostname );
					if ( host)
					{
						memcpy(&server.sin_addr , host->h_addr_list[0] , host->h_length );
					}
					else
						return false;
				}

				if ( SOCKET_ERROR == ::connect( handle_ , (struct sockaddr*)& server , sizeof( server)))
				{
					close();
					return false;
				}

				return true;
			}
		}

		/// 读取数据
		streamsize read( char *  buff , streamsize  bufsize)
		{
			streamsize x = ::recv( handle_ , buff , bufsize , 0 );
			if ( x == SOCKET_ERROR )
			{
				close();
				int error = ::WSAGetLastError();
				return 0;
			}
			return x;
		}

	private:
		/// 网络套接字			
		mutable SOCKET handle_;
		/// 地址族  IPV4 ， 6
		int     addr_family_;

	};

}