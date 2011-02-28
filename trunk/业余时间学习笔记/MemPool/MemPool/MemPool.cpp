// MemPool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MemFactory.h"
//#include "ObjTest.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <fstream>
#include <limits>
#include <mmsystem.h>

#pragma comment( lib , "winmm.lib")
class A
{
public:
	int a;
};


int _tmain(int argc, _TCHAR* argv[])
{
	/// 测试内存分配
	CMemFactory  Memory;
	
	/*
	 * 基于对象的多参数内存池分配测试
	 */
// 	A *a = Memory.Alloc<A,int,float,char*>(sizeof(A),1,2,"a");
// 	Memory.FreeObj<A>(a,sizeof(A));
//	A* a = Memory.Alloc<A>(sizeof(A));
// 
// 	B *b = Memory.Alloc<B,int>(sizeof(B),10);
// 	Memory.FreeObj<B>(b,sizeof(B));
// 
// 	return 0;
	//freopen("data.out","w",stdout);
	srand( (long)time(NULL) );
	
	///  测试
	std::map<void*,long>  m_Cachelist;
	std::map<void*,long>  m_dynamiclist;

	long time1 = 0;
	long time2 = 0,time3,time4;
	///  分配大小
	long  arr[ ] = { 32,64,128,256,512,1024,2048,5120,5130,7000,6000,10240,15000,20000};
	long  Count[13]={0};
	
	///   被分配的概率
	long  Num[ ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,8,8,8,8,9,9,9,10,10,11,11,12,12};

	for ( long c = 0 ; c < 10000000 ; c ++ )
	//while( 1 )
	{
		long  size = sizeof(Num)/sizeof(long);
		long  rad  = rand()%(size);
		long  Asize= arr[Num[rad]] ;

		time3 = timeGetTime( );	
		void* p1 = Memory.Alloc( Asize );
		time4 = timeGetTime( );

		time1 += (time4-time3);


		time3 = timeGetTime( );	
		void* p2 = new char[Asize];
		time4 = timeGetTime( );

		time2 += (time4-time3);


		Count[ Num[rad] ] ++;

		memset( p1 , 0 , Asize );
		memset( p2 , 0 , Asize );

		m_Cachelist[p1] = Asize;
		m_dynamiclist[p2] = Asize;
 
 		if ( m_Cachelist.size() >= 90000 )
 		{
  			size_t  lsize = m_Cachelist.size();
			std::map<void*,long>::iterator it =m_Cachelist.begin()   ;
			for ( size_t i = 0; i < 60000 ; i++ )
 			{
				time3 = timeGetTime( );
 				Memory.Free(it->first );
				time4 = timeGetTime( );
				time1 += (time4-time3);
 				m_Cachelist.erase( it++ );
 			}

			lsize = m_dynamiclist.size();
			it    =  m_dynamiclist.begin()   ;
			for ( size_t i = 0; i < 60000 ; i++ )
			{				
				time3 = timeGetTime( );
				delete it->first;
				time4 = timeGetTime( );
				time2 += (time4-time3);
				m_dynamiclist.erase( it++ );
			}
 		}			    
		if(c> 0 && c % 1000000 == 0 )
			std::cout << c << std::endl;
			//Memory.Print();
	}

	std::cout <<" time 1 " << time1 <<" time 2 "<< time2 << std::endl;
	for ( int i = 0 ; i < 13 ; i++)
	{
		std::cout <<" Count[ " << i+1 <<" ] = " << Count[ i ] << std::endl;
	}

	return 0;
}

