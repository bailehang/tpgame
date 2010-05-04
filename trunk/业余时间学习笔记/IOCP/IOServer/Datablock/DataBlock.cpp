// DataBlock.cpp : 定义控制台应用程序的入口点。
/*============================================
*
* file:   DataBlockSetWriteRead.h
*
* brief:  main 函数测试
*
* Authtor:Expter
*
* Data:   20090618
*
* Mend:   //
===========================================*/

#include "stdafx.h"

/*
#include "DataBlockSetWriteRead.h"
int _tmain(int argc, _TCHAR* argv[])
{
	CDataBlockSetWrite SetWriteDB;

	DBWriteSet(SetWriteDB);
	SetWriteDB.AddtoByteArray(30);
	
	char str[50] = "this is only test ! ";
	char  tmp[10001];
	
	for(int  i = 0 ; i < sizeof(tmp) ;  i++)
		tmp[i] = '1';
	tmp[10000] = '\0';
	SetWriteDB.AddtoByteArray(str);
	SetWriteDB.AddtoByteArray(tmp);

	SetWriteDB.PutAllInfo();


	int  lNum = 0;
	DataBlockSetRead  SetReadDB;
	DBReadSet(SetReadDB);
	lNum = SetReadDB.GetDataFromByteArray<long>();
	char * p1 = SetReadDB.GetBufferFromByteArray(str,256);
	char * p2 = SetReadDB.GetBufferFromByteArray(tmp,10010);
	
	printf("lNum = %d\n str = %s\n tmp = %s\n",lNum,p1,p2);


	return 0;
}

*/