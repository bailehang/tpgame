
#pragma  once 

#include "Pool\BlockPool.h"
#include "Pool\HeapPool.h"
#include "Factory.h"
#include "Pool\SamllObjAllocator.h"
#include <set>
using namespace std;

class CMemFactory
{
	typedef  void* (SmallObjAllocator::*AllocMem)();
	typedef  bool  (SmallObjAllocator::*Destory)(void*);

	typedef  std::set<void*>	SetMap;

public:
	CMemFactory();

	void  Init();

	~CMemFactory()
	{
		if(m_factory)
		{
			delete m_factory;
			m_factory = NULL;
		}
	};

	void*     Alloc(unsigned long size);

	void      Free(void* pAddr, unsigned long size);

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
	void     FreeObj( T * pt , unsigned long size) 
	{ 
		if( !pt ) return ;
		pt->~T(); 

		Free(pt,size);
	} 

	int		  Index(unsigned long size);
	
private:

	/// 二种分配方式
	//BlockPool  m_Block;
	//HeapPool   m_Heap;

	Factory<long,SmallObjAllocator*,AllocMem,Destory>* m_factory;

	SetMap				m_setList;
};