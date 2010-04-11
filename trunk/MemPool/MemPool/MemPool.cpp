// MemPool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Pool\BlockPool.h"
#include <stdlib.h>
#include <list>

int _tmain(int argc, _TCHAR* argv[])
{
	BlockPool  Pool;
	Pool.InitMemoryAlloc();

	std::list<void*>  m_Cachelist;
	long  arr[ ] = { 32,64,128,256,512,1024,2048,5120 };
	long  Count[8]={0};
	long  Num[ ] = { 0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,7,7,7};
	for ( long c = 0 ; c < 1000000 ; c ++ )
	{
		long  size = sizeof(Num)/sizeof(long);
		long  rad  = rand()%size;
		long  Asize= arr[Num[rad]] + (Num[rad]==3 ? 1 : 0);
		void* p =Pool.Alloc( Asize )	;

		Count[ Num[rad] ] ++;
		memset( p , 0 , Asize )	;

		//if( Num[rad] >= 3)
		m_Cachelist.push_back( p );

		if ( m_Cachelist.size() >= 10000 )
		{
			//std::cout <<" 缓存有10000 个,进行释放" << std::endl;
			size_t  lsize = m_Cachelist.size();
			std::list<void*>::iterator it =m_Cachelist.begin()   ;
			for ( int i = 0; i < 10000 ; i++ )
			{
				 Pool.Free( *it/*m_Cachelist[i]*/ );
				 it = m_Cachelist.erase( it );
			}
		}
	}

	for ( int i = 0 ; i < 8 ; i++)
	{
		std::cout <<" Count[ " << i+1 <<" ] = " << Count[ i ] << std::endl;
	}
	return 0;
}

