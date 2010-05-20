
#pragma  once 

#include "FreeList.h"
#include <vector>
using namespace std;

class   SmallObjAllocator
{
public:
	SmallObjAllocator( short  chunkSize ,
					   long   MaxObjectSize)
	{
		m_blockSize = chunkSize;
		if( MaxSize < MaxObjectSize )
		{
			MaxObjectSize = MaxSize;
		}
		m_ObjSize   = MaxObjectSize;
		//std::cout << MaxSize <<"  " << m_ObjSize << std::endl;
		m_alloChunk = NULL;
	}

	~SmallObjAllocator()
	{
		Chunks::iterator i = m_chunk.begin();
		for ( ; i != m_chunk.end(); ++i)
		{
			delete *i;
		}
		m_chunk.clear();
	}

	void*  Allocate()
	{
		/// ÄÚ´æ²»×ã
		if ( !m_alloChunk || m_alloChunk->m_availSize == 0 )
		{
			Chunks::iterator i = m_chunk.begin();
			for ( ; ; ++i)
			{
				if ( i == m_chunk.end() )
				{
					m_chunk.push_back( new Chunk(m_blockSize,m_ObjSize) );
					m_alloChunk = m_chunk.back();

					std::cout <<" Chucks size "<< m_chunk.size() ;
					
					break;
				}
				if ( (*i)->m_availSize > 0 )
				{
					m_alloChunk = *i;
					break;
				}
			}
		}

		//assert( m_alloChunk != 0 );
		//assert( m_alloChunk->m_availSize != 0 );
		return m_alloChunk->Allocate();
	}
	bool   Destory(void* pData)
	{
		Chunks::iterator it =  m_chunk.begin();
		for( ; it != m_chunk.end() ; it++ )
		{
			if( (*it)->Check( pData ) )
			{
				return (*it)->Release( pData );
			}
		}
		return false;
	}

private:
	typedef std::vector<Chunk*>   Chunks;
	Chunks				m_chunk;

	Chunk*				m_alloChunk;
	Chunk*				m_freeChunk;
	
	short				m_blockSize;
	short				m_ObjSize;
};