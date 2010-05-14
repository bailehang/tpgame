
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

	int		  Index(unsigned long size);
	
private:

	/// 二种分配方式
	//BlockPool  m_Block;
	//HeapPool   m_Heap;

	Factory<long,SmallObjAllocator*,AllocMem,Destory>* m_factory;

	SetMap				m_setList;
};