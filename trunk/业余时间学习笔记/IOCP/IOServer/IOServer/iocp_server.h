#pragma once
#include "stdafx.h"
#include "cmio_socket.h"
#include "register_list.h"
#include "CBassMessage.cpp"

namespace _tp_io_complete_portname
{
	class iocp_server
	{
		typedef iocp_server self_type;
		typedef unsigned  long uint32_t;

	public:
		iocp_server(void)
		{

		}
		
		~iocp_server(void)
		{
				close();
		}
	
		
	public:
		void close(){
			if ( thandle_ )
			{
				::WaitForSingleObject(thandle_, INFINITE);
				thandle_ = 0;
			}
		}

		void open()
		{
			unsigned int tid; 
			thandle_ = (HANDLE)::_beginthreadex( 0 , 0 , _m_serv_thread , this , 0 , & tid);
		}
		
		static void _m_handler( int type , SOCKET s , void * socket_extra , const char * buf , unsigned long bufsize , void * userdata )
		{
			if ( userdata == 0)
				return;

			self_type &self = *reinterpret_cast<self_type*>(userdata);

			//std::cout <<" _m_handler " << std::endl;
			///
			/// recv data ,接收所有的数据
			///
			if ( type == 1 ) 
			{
				DWORD usedsize = 0;
				DWORD rest_size= bufsize;
				
				///
				/// 转换为msg 
				///
			//	_tp_message_load::CBassMessage  basemsg;
			//	basemsg = *reinterpret_cast<_tp_message_load::CBassMessage*>(buf);
			//	int len =  bufsize;



				/*
				while( usedsize < bufsize )
				{
					if ( rest_size >= 0 )
					{
							/// recv data
						}
						else
							break;
					}
				*/
			}	
			///
			/// user dis connect 
			///
			else if ( type == 0 )
			{
				if ( socket_extra)
				{
					/// dis client connect
					register_list::item_type * regitem = reinterpret_cast<register_list::item_type*>(socket_extra);

					self.reglist_.erase(s);
				}
			}
		}

		static void * _m_socket_extra_maker( SOCKET s , void * arg )
		{
			if ( arg )
			{
				 reinterpret_cast<self_type*>(arg)->reglist_.push_back(s);
				 return reinterpret_cast<self_type*>(arg)->reglist_.find(s);
			}
			return 0;
		}


		bool  _m_send_msg_toclient(const char * buf /*_tp_message_load::CBassMessage  msg*/ )
		{
// 			typedef std::list<register_list::item_type > itemtype;
// 			register_list::item_type  item(0,0,0,0);
// 			item =this->reglist_.GetItems();
			set<SOCKET>  s =_tp_io_complete_portname::cmio_socket::GetFirstSocket();
			for ( set<SOCKET>::iterator it = s.begin() ; it != s.end() ; ++it  )
			{
				long  sock = *it;
				::send( sock, buf , strlen( buf ) , 0 );
			}
				

			return 0;
		}
	private:
		static  unsigned __stdcall _m_serv_thread( void * arg)
		{
			self_type &self = *reinterpret_cast<self_type*>(arg);
			
			cmio_socket & mscoket = self.msocket_;
			mscoket.listener( 2048 , 5);
			mscoket.handler( &self_type::_m_handler , arg);

			while ( 1 )
			{
				mscoket.accept( _m_socket_extra_maker , arg );
			}
			
			_endthreadex( 0 );
			return 0;
		}
	private:
		HANDLE			thandle_;
		cmio_socket		msocket_;
		register_list   reglist_;
		

	};
}