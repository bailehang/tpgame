/**
 *	@brief  关于chunk的一个封装类，主要管理freelist数据
 *  @author expter
 *  @date   2010.05.10
 */

#pragma  once 

#include "FreeList.h"
#include <list>
using namespace  std;

class  Chunk 
{
	typedef Chunk	self_type;
	typedef std::list<List*,Allocator<List*> >	list_type;

public:
	Chunk( long size, long count );

	~Chunk();

	void*  Alloctor();
	
	bool   Free( void *pData );

private:

	void   Init();

	void   Clean();

private:

	list_type					m_list;
	List						*m_alloc;
	
	long						m_blockSize;
	long						m_blockcount;
};