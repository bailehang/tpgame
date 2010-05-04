#include "stdafx.h"
#include <winsock2.h>
#include "locks.h"

namespace net
{
	class wsa_life
	{
	public:
		wsa_life()
		{
			WSADATA wsa_data;
			::WSAStartup(MAKEWORD( 2, 2 ), &wsa_data);
		}

		~wsa_life()
		{
			::WSACleanup();
		}
	};


	class socket_type
	{
		typedef socket_type self_type;
	public:
		typedef unsigned streamsize;
		struct values
		{
			enum
			{
				null_socket = INVALID_SOCKET,
			};
		};

		socket_type()
			:handle_(values::null_socket)
		{}

		socket_type(SOCKET s, int addr_family)
			:handle_(s), addr_family_(addr_family)
		{
		}

		socket_type(int address_family, int protocol, bool is_stream)
			:handle_(values::null_socket)
		{
			open(address_family, protocol, is_stream);
		}

		socket_type(const self_type& rhs)
			:handle_(rhs.handle_), addr_family_(rhs.addr_family_)
		{
			rhs.handle_ = values::null_socket;
		}

		self_type& operator=(const self_type& rhs)
		{
			if(this != &rhs)
			{
				handle_ = rhs.handle_;
				addr_family_ = rhs.addr_family_;
			}
			return *this;	
		}

		~socket_type()
		{
			close();	
		}

		bool empty() const
		{
			return (handle_ == values::null_socket);
		}

		bool open(int address_family, int protocol, bool is_stream)
		{
			close();

			handle_ = ::socket(address_family, (is_stream?SOCK_STREAM:SOCK_DGRAM), protocol);

			addr_family_ = address_family;

			return (handle_ != values::null_socket);
		}

		void close()
		{
			if(values::null_socket != handle_)
			{
				::closesocket(handle_);
				handle_ = values::null_socket;
			}
		}

		bool connet(const char* hostname, int port)
		{
			if(handle_ != values::null_socket)
			{
				struct sockaddr_in server;
				server.sin_family = addr_family_;
				server.sin_port = ::htons(port);
				server.sin_addr.s_addr = ::inet_addr(hostname);

				if(server.sin_addr.s_addr == INADDR_NONE)
				{
					struct hostent* host = ::gethostbyname(hostname);

					if(host)
						memcpy(&server.sin_addr, host->h_addr_list[0], host->h_length);
					else
						return false;
				}

				return (SOCKET_ERROR != ::connect(handle_, (struct sockaddr*)&server, sizeof server));
			}
			return false;
		}

		streamsize read(char * buffer, streamsize bufsize)
		{
			streamsize x = ::recv(handle_, buffer, bufsize, 0);
			if(x == SOCKET_ERROR)
			{
				int error = ::WSAGetLastError();
				error = 5;
				return 0;
			}
			return x;
		}

		streamsize write(const char * buffer, streamsize bufsize)
		{
			streamsize x = ::send(handle_, buffer, bufsize, 0);
			if(x == SOCKET_ERROR)
			{
				int error = ::WSAGetLastError();
				return 0;
			}

			return x;
		}
	private:
		mutable SOCKET	handle_;
		int		addr_family_;

	};//end class client_socket_type

	class listener_type
	{
	public:
		struct values
		{
			enum
			{
				null_socket = INVALID_SOCKET,
			};
		};

		listener_type()
			:handle_(values::null_socket)
		{}

		listener_type(int address_family, int protocol, bool is_stream)
			:handle_(values::null_socket)
		{
		}

		bool empty() const
		{
			return (handle_ == values::null_socket);
		}

		bool open(int address_family, int protocol, bool is_stream)
		{
			close();

			handle_ = ::socket(address_family, (is_stream?SOCK_STREAM:SOCK_DGRAM), protocol);

			addr_family_ = address_family;

			return (handle_ != values::null_socket);
		}

		void close()
		{
			if(values::null_socket != handle_)
			{
				::closesocket(handle_);
				handle_ = values::null_socket;
			}
		}

		bool listen(int port, int connections)
		{
			if(handle_ == values::null_socket && connections > 0)	return false;

			struct sockaddr_in	saddr;
			saddr.sin_family = AF_INET;
			saddr.sin_port = ::htons(port);
			saddr.sin_addr.s_addr = ::htonl(INADDR_ANY);

			if(::bind(handle_, reinterpret_cast<sockaddr*>(&saddr), sizeof saddr))
				return false;

			return (::listen(handle_, connections) != 0);
		}

		socket_type accept()
		{
			if(handle_ != values::null_socket)
			{
				sockaddr_in saddr;
				int size = sizeof saddr;

				SOCKET s = ::accept(handle_, reinterpret_cast<sockaddr*>(&saddr), & size);
				if(s != values::null_socket)
					return socket_type(s, saddr.sin_family);
			}

			return socket_type();	
		}
	private:
		SOCKET handle_;
		int addr_family_;
	};
}//end namespace socket
