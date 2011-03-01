
#pragma  once 

#include "Pool\BlockPool.h"
#include "Pool\HeapPool.h"
#include "Pool\SamllObjAllocator.h"
#include "Pool\DynamicPool.h"
#include "Factory.h"
#include <set>
#include <map>
using namespace std;

class CMemFactory
{
	typedef  void* (SmallObjAllocator::*AllocMem)();
	typedef  bool  (SmallObjAllocator::*Destory)(void*);

	typedef  std::set<void*>						SetMap;
	typedef  std::map<long,DynamicPool* >			DynamicMap;

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

	void      Free(void* pAddr, unsigned long size = 0);

	template< class T >
	T * Alloc(unsigned long lSize)
	{
		void* ptMem = Alloc(lSize);
		if( !ptMem) return NULL; 
		T * pt = new(ptMem)T ;
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

	int		  Index(unsigned long size);

	void      Print()
	{
		m_factory->Print();
	}
	
private:

	/// 二种分配方式
	//BlockPool  m_Block;
	//HeapPool   m_Heap;

	Factory<long,SmallObjAllocator*,AllocMem,Destory>* m_factory;

	DynamicMap				m_dynamic;
	AllocMem				m_alloc  ;
	Destory					m_destory;
	Dynamic				m_mapList;
};