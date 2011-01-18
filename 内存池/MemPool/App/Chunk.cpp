#include "stdafx.h"
#include "Chunk.h"

Chunk::Chunk( long size, long count )
{
	if( count > MaxSize )
		count = MaxSize;

	m_blockSize = size;
	m_blockcount= count;

	Init();
}

Chunk::~Chunk()
{
	//std::cout << " ~Chunk() Chunk Size =%d " << m_list.size() << std::endl;

	list_type::iterator i = m_list.begin();

	for ( ; i != m_list.end(); ++i)
	{
		delete *i;
	}
	m_list.clear();
}

void*  Chunk::Alloctor()
{
	if ( !m_alloc || m_alloc->Full() )
	{
		list_type::iterator it = m_list.begin();
		for ( ; ; ++it )
		{
			if ( it == m_list.end() )
			{
				Init();
				break;
			}
			if ( !m_alloc->Full() )
			{
				m_alloc = *it;
				break;
			}
		}
	}

	assert( m_alloc && !m_alloc->Full() );

	return m_alloc->Allocator();
}

bool   Chunk::Free( void *pData )
{
	list_type::iterator it = m_list.begin();
	for ( ; it != m_list.end() ; it ++ )
	{
		if ( (*it)->Check(pData) )
		{
			(*it)->Free( pData );

			Clean();
			return true;
		}
	}

	Clean();
	return false;
}

void  Chunk::Clean()
{
	list_type::iterator it = m_list.begin();
	for ( ; it != m_list.end() ;  )
	{	
		if ( m_list.size() * CHUNKSIZE < MAXALLOCSIZE )
			break;

		if ( (*it)->Empty() )
		{
			List *plist = *it;
			delete plist;	
			plist = NULL;
			it = m_list.erase( it );
		}
		else
			it ++;
	}
}

void    Chunk::Init()
{
	m_alloc = new List( m_blockSize , m_blockcount );

	m_list.push_back( m_alloc );

	assert ( m_alloc );
}