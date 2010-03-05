// CTimer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include "timer.h"
using namespace std;

long  Sum1= 0 ;
long  Sum2= 0 ;
long  Sum3= 0 ;
long  Sum = 0;

class  CTimerTest : public   CGameEvent
{
public:
	long TimeOut( eTimerType type)
	{
		switch ( type)
		{
		case eTimer1:
			std::cout <<"Sum1 = "<< Sum1 ++ << std::endl;
			break;
		case eTimer2:
			std::cout <<"Sum2 = "<< Sum2 ++ << std::endl;
			break;
		case eTimer3:
			std::cout <<"Sum3 = "<< Sum3 ++ << std::endl;
			break;
		default:
			std::cout <<"Sum  = "<< Sum ++ << std::endl;
			break;
		}
		return 0;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	CTimer  mytimer( 40 );
	
	long    n;
	std::cin >> n;
	CTimerTest test;
	for ( int i = 0 ; i < n ; i++ )
	{
		timernode* tim = new timernode ;
		tim->expires  = 0;
		tim->etime    = GetCurrSystemTime() + (rand() % 1000 ) * 6;
		tim->pFun     =&test;
		tim->eType    =(eTimerType)( i%3 + 10 );

		mytimer.add_timer( tim );
	}

	for ( ;; )
	{
		if ( (Sum1 + Sum2 + Sum3) == n )
			break;

		/// 运行所有的定时器
		mytimer.Expires( GetCurrSystemTime() );
	}

	std::cout << " sum1 = " << Sum1 
		      << " sum2 = " << Sum2 
			  << " sum3 = " << Sum3 
			  << " sum  = " << Sum ;
	return 0;
}
