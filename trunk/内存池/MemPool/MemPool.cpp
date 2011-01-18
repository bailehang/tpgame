// MemPool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MemFactory.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <fstream>
#include <limits>
#include <mmsystem.h>
#include <iostream>
using namespace std;

#pragma comment( lib , "winmm.lib")

class A
{
public:
	A()
	{
		a = 0 ;
		std::cout << "A: a = " << a << std::endl;
	}
	int a;
};

class B
{
public:
	B(int b) : a( b )
	{
		std::cout << "B: a = " << a << std::endl;
	}
	~B()
	{
		std::cout <<"B~ UnConStruction " << std::endl;
	}

	int a ;
};

void  ObjAllocTest()
{
	MemFactory  Memory;

	A* a = Memory.Alloc<A>( );
	B* b = Memory.Alloc<B,int>( 2 );

	Memory.FreeObj( a );
	Memory.FreeObj( b );
}

void  MemTest()
{
	MemFactory  Memory;

	///  测试
	std::map<void*,long>  m_Cachelist;
	std::list<void*>      m_dynamiclist;

	long  time1 = 0;
	long  time2 = 0,time3;
	///  分配大小
	long  arr[ ] = { 32,64,128,256,512,1024,2048,5120,5130,7000,6000,10240,15000,20000};
	long  Count[13]={0};

	///   被分配的概率
	long  Num[ ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,8,8,8,8,9,9,9,10,10,11,11,12,12};

	for ( long c = 0 ; c < 10000000 ; c ++ )
	{
		long  size = sizeof(Num)/sizeof(long);
		long  rad  = rand()%(size);
		long  Asize= arr[Num[rad]] ;

		time3 = timeGetTime( );	
		void* p1 = Memory.Alloc( Asize );
		time1 += (timeGetTime( )-time3);

		Count[ Num[rad] ] ++;
		memset( p1 , 0 , Asize );
		m_dynamiclist.push_back( p1 );

		if ( m_dynamiclist.size() >= 100000 )
		{
			size_t lsize = m_dynamiclist.size();
			std::list<void*>::iterator  it = m_dynamiclist.begin()   ;
			for ( size_t i = 0; i < 60000 ;  i++)
			{
				time3 = timeGetTime( );
				Memory.Free( *it );
				time2 += (timeGetTime( )-time3);

				it = m_dynamiclist.erase( it );
			}
		}
		if(c> 0 && c % 1000000 == 0 )
			std::cout << c << std::endl;
	}

	std::cout <<"MemPool Alloc: time 1 " << time1 <<" time 2 "<< time2 << std::endl;
	for ( int i = 0 ; i < 13 ; i++)
	{
		std::cout <<" Count[ " << i+1 <<" ] = " << Count[ i ] << std::endl;
	}
}

void  SysTest()
{
	///  测试
	std::map<void*,long>  m_Cachelist;
	std::list<void*>      m_dynamiclist;

	long  time1 = 0;
	long  time2 = 0,time3;
	///  分配大小
	long  arr[ ] = { 32,64,128,256,512,1024,2048,5120,5130,7000,6000,10240,15000,20000};
	long  Count[13]={0};

	///   被分配的概率
	long  Num[ ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,8,8,8,8,9,9,9,10,10,11,11,12,12};

	for ( long c = 0 ; c < 10000000 ; c ++ )
	{
		long  size = sizeof(Num)/sizeof(long);
		long  rad  = rand()%(size);
		long  Asize= arr[Num[rad]] ;

		time3 = timeGetTime( );	
		void* p1 = new char[Asize];
		time1 += (timeGetTime( )-time3);

		Count[ Num[rad] ] ++;
		memset( p1 , 0 , Asize );
		m_dynamiclist.push_back( p1 );

		if ( m_dynamiclist.size() >= 100000 )
		{
			size_t lsize = m_dynamiclist.size();
			std::list<void*>::iterator  it = m_dynamiclist.begin()   ;
			for ( size_t i = 0; i < 60000 ;  i++)
			{
				time3 = timeGetTime( );
				delete *it;
				time2 += (timeGetTime( )-time3);

				it = m_dynamiclist.erase( it );
			}
		}
		if(c> 0 && c % 1000000 == 0 )
			std::cout << c << std::endl;
	}

	std::list<void*>::iterator  it = m_dynamiclist.begin() ;
	for ( ; it != m_dynamiclist.end() ; it ++ )
	{
		time3 = timeGetTime( );
		delete *it;
		time2 += (timeGetTime( )-time3);
		it = m_dynamiclist.erase( it );
	}
	std::cout <<"System Alloc: time 1 " << time1 <<" time 2 "<< time2 << std::endl;
	for ( int i = 0 ; i < 13 ; i++)
	{
		std::cout <<" Count[ " << i+1 <<" ] = " << Count[ i ] << std::endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand( (long)time(NULL) );

	ObjAllocTest();

	MemTest();

	SysTest();

	return 0;
}

