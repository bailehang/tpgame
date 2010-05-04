// IOServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iocp_server.h"
#include "CBassMessage.cpp"
#include "iocp_server.h"


int _tmain(int argc, _TCHAR* argv[])
{
	_tp_io_complete_portname::iocp_server ioserver;

	ioserver.open();

// 	Sleep( 2000);
// //	SOCKET  s = _tp_io_complete_portname::cmio_socket::GetFirstSocket();
// 	for ( ; ;)
// 	{
// 			ioserver._m_send_msg_toclient(  "this is that server send to client msg！" );
// 			Sleep(1000);
// 	}

	return 0;
}

