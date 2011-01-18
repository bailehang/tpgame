
#pragma  once 

#include "App/Chunk.h"
#include "App/Factory.h"

#include <map>

using namespace std;


class  MemFactory
{
	typedef void* (Chunk::*Alloctor)( );
	typedef bool  (Chunk::*FreeFun)(void*);

	typedef Factory<long,Chunk*,Alloctor,FreeFun> Factorymgr;

public:
	MemFactory();

	~MemFactory();

	void	Init();

	void*   Alloc( unsigned long size );

	bool    Free(void* pAddr, unsigned long size = 0 );

	template< class T >
	T * Alloc()
	{
		unsigned long lSize = sizeof(T);
		void* ptMem = Alloc(lSize);
		if( !ptMem) return NULL; 
		T * pt = new(ptMem) T ;
		return pt;
	}

	DEFINE_CALL_CON(1); 
	DEFINE_CALL_CON(2); 
	DEFINE_CALL_CON(3); 
	DEFINE_CALL_CON(4); 
	DEFINE_CALL_CON(5); 
	DEFINE_CALL_CON(6); 
	DEFINE_CALL_CON(7); 
	DEFINE_CALL_CON(8); 
	DEFINE_CALL_CON(9); 
	DEFINE_CALL_CON(10);

	template <class T> 
	void     FreeObj( T * pt , unsigned long size = 0) 
	{ 
		if( !pt ) return ;
		pt->~T(); 

		Free(pt,size);
	}

private:
	Factorymgr		*m_memfactory;
	Alloctor		m_alloc ;
	FreeFun			m_destory;
};