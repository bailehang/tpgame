// IOClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "socket.cpp"
#include <time.h>
#include <iostream>
using namespace std;
//#include "..\Datablock\CBassMessage.cpp"

#define  NowTime  (long)time(NULL);
int _tmain(int argc, _TCHAR* argv[])
{
	netpack::wsa_life		wsa;

	netpack::socket_type	ioclient;

	ioclient.open( AF_INET , 0 , true);

	ioclient.connect("127.0.0.1",2048);

	//_tp_message_load::CBassMessage  msg;
	//msg.writeinit();
	
	//msg.AddtoByteArray(20);
	//msg.AddtoStrByteArray("this is test");

	long start = NowTime;
	int i = 0 ;
	for ( ; i++ < 10000 ; )
	{
		int wlen = ioclient.write(	"this is test1", 50);	
		if ( i % 7 == 0)
		{
			Sleep(1);
		}
	}
	int wlen = ioclient.write("end", 50);	
// 	for ( ;; )
// 	{
// 		char str[100];
// 		int wlen = ioclient.read( str  , 100);
// 		std::cout << " str is " << str << std::endl;
// 	}
	long endt = NowTime;
	
	std::cout << " test time  = " << endt-start <<std::endl;


	return 0;
}

