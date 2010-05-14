// priority_queue.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <queue>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <functional>
#include <time.h>

using namespace std;

/*
*	 Define	TimerType
*/
enum    TimerType
{
	ObjType_Player	= 0x01,
	ObjType_Pet,
	ObjType_Monster,
};

/*
*	 一个定时器结点
*/
struct  tagTimerVar
{
	tagTimerVar(){};
	tagTimerVar( long startTime , long interval , long  tid , TimerType type , void * Obj )
	{
		StartTime = startTime;
		Interval  = interval;
		TimerId   = tid ;
		Type      = type;
		pObj      = Obj;
	}
	long		StartTime;
	long		Interval;
	long		TimerId;
	TimerType	Type;
	void*		pObj;
};

//typedef list<tagTimerVar*>   tagTimer;

struct cmp
{
	bool operator() ( const tagTimerVar* t1 , const tagTimerVar* t2  )
	{
		return t1->StartTime > t2->StartTime; 
	}
};

typedef priority_queue<tagTimerVar* , vector<tagTimerVar*> , cmp >  pqueue;

//typedef priority_queue<int , vector<int> , greater<int>  >  pq;


int _tmain(int argc, _TCHAR* argv[])
{
	pqueue  test;
	
	long    lt = time( NULL );

	srand( time(NULL ) );
	tagTimerVar *pp[4000];
	for ( int i = 0 ; i < 2000 ; i++ )
	{
		int  idx =  rand() % 2; 
		char str[ 40 ]; 
		sprintf( str ,"t%d" , i+1 );
		pp[i] = new tagTimerVar( time(NULL)+idx , idx , i+1 , ObjType_Player , str);
		test.push( pp[i] );
	}
// 	tagTimerVar *t1 = new tagTimerVar( time(NULL)+5 , 5 , 1 , ObjType_Player , "t1" );
// 	tagTimerVar *t2 = new tagTimerVar( time(NULL)+1 , 1 , 2 , ObjType_Player , "t2" );
// 	tagTimerVar *t3 = new tagTimerVar( time(NULL)   , 0 , 3 , ObjType_Player , "t3" );
// 	tagTimerVar *t4 = new tagTimerVar( time(NULL)+3 , 3 , 4 , ObjType_Player , "t4" );
// 	tagTimerVar *t5 = new tagTimerVar( time(NULL)   , 0 , 5 , ObjType_Player , "t5" );
// 	tagTimerVar *t6 = new tagTimerVar( time(NULL)   , 2 , 6 , ObjType_Player , "t6" );

	//test.push( t1 );
// 	test.push( t2 );
// 	test.push( t3 );
// 	test.push( t4 );
// 	test.push( t5 );
// 	test.push( t6 );

	while( !test.empty() )
	{
		tagTimerVar * pt = test.top();
		// 没有超时的数据
		lt = time( NULL );
		//std::cout <<" ----------------------------------------\n" ;
		if ( pt->StartTime > time(NULL) )
			continue;

		test.pop();
		cout << " id = " << pt->TimerId <<" startime " << pt->StartTime  <<" val = " << (char*)pt->pObj << endl;
		// 循环周期
		if ( pt->Interval> 0 )
		{
			pt->StartTime = time(NULL) + pt->Interval;
			test.push( pt );
		}
		else
		{
			delete pt;
		}
		
	}

	return 0;
}

