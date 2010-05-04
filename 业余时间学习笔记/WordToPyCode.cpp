// WordToPyCode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <windows.h>
#include <wchar.h>  
#include <iosfwd>
#include <crtdbg.h>

using namespace std;

#define _CRTDBG_MAP_ALLOC   //并非绝对需要该语句，但如果有该语句，打印出来的是文件名和行数等更加直观的信息

void FirstLetter(int nCode, string& strLetter)
{
   if(nCode >= 1601 && nCode < 1637) strLetter =  ("A");
   if(nCode >= 1637 && nCode < 1833) strLetter =  ("B");
   if(nCode >= 1833 && nCode < 2078) strLetter =  ("C");
   if(nCode >= 2078 && nCode < 2274) strLetter =  ("D");
   if(nCode >= 2274 && nCode < 2302) strLetter =  ("E");
   if(nCode >= 2302 && nCode < 2433) strLetter =  ("F");
   if(nCode >= 2433 && nCode < 2594) strLetter =  ("G");
   if(nCode >= 2594 && nCode < 2787) strLetter =  ("H");
   if(nCode >= 2787 && nCode < 3106) strLetter =  ("J");
   if(nCode >= 3106 && nCode < 3212) strLetter =  ("K");
   if(nCode >= 3212 && nCode < 3472) strLetter =  ("L");
   if(nCode >= 3472 && nCode < 3635) strLetter =  ("M");
   if(nCode >= 3635 && nCode < 3722) strLetter =  ("N");
   if(nCode >= 3722 && nCode < 3730) strLetter =  ("O");
   if(nCode >= 3730 && nCode < 3858) strLetter =  ("P");
   if(nCode >= 3858 && nCode < 4027) strLetter =  ("Q");
   if(nCode >= 4027 && nCode < 4086) strLetter =  ("R");
   if(nCode >= 4086 && nCode < 4390) strLetter =  ("S");
   if(nCode >= 4390 && nCode < 4558) strLetter =  ("T");
   if(nCode >= 4558 && nCode < 4684) strLetter =  ("W");
   if(nCode >= 4684 && nCode < 4925) strLetter =  ("X");
   if(nCode >= 4925 && nCode < 5249) strLetter =  ("Y");
   if(nCode >= 5249 && nCode < 5590) strLetter =  ("Z");
}


void GetFirstLetter(char * strName, string& strFirstLetter)
{
    unsigned char ucHigh, ucLow;
    int nCode;
    string strRet;

    strFirstLetter.empty();

	for (int i=0; i<  (int)strlen(strName) ; i++)
    {
        if ( (unsigned char)strName[i] < 0x80 )
            continue;

		ucHigh = (unsigned char)(strName[i]);
        ucLow  = (unsigned char)(strName[i+1]);
       
        if ( ucHigh < 0xa1 || ucLow < 0xa1)
            continue;
        else
            // Treat code by section-position as an int type parameter,
            // so make following change to nCode.
            nCode = (ucHigh - 0xa0) * 100 + ucLow - 0xa0;

        FirstLetter(nCode, strRet);
        strFirstLetter += strRet;
        i++;
    }
}





int _tmain(int argc, _TCHAR* argv[])
{
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
 	_CrtSetDbgFlag( tmpFlag );

	string strRes;
    char *p  =  "这是一个A测试[BOOST]学习使用点滴-2-Timer库";
    GetFirstLetter(p, strRes);

	std::cout << strRes << std::endl;
   
	//Then the value of strRes is "ZSYGCS".
	return 0;
}

