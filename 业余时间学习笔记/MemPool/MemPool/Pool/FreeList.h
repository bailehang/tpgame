
#pragma  once 

#define  MaxSize  ((1<<15)-1)
class Chunk
{
	friend class SmallObjAllocator;
public:

	Chunk( int blocksize, long  len )
	{
		//ReleaseAll();

		if( MaxSize < len )
		{
			len = MaxSize;
		}
		m_blockSize = blocksize+4;
		m_availSize = len;
		m_Length  = len;
		m_firstBlock = 0;

		m_pData = (char*)VirtualAlloc(NULL,m_blockSize * len,MEM_COMMIT,PAGE_READWRITE);

		char* p = m_pData;
		for ( long i = 0 ; i < len ; p+=m_blockSize)
		{
			*(long*) p = ++i;
		}
	}

	~Chunk()
	{
		ReleaseAll();		
	}

	void ReleaseAll()
	{
		std::cout << " ~Chuck 析构函数调用 Size = " <<  m_blockSize - 4 << std::endl ;
		m_blockSize = m_firstBlock = m_availSize = 0;
		if( !m_pData )
		{
			delete m_pData;
			m_pData = NULL;
		}
	}

	void*  Allocate( )
	{
		if ( m_availSize <= 0 ) return NULL;
		
		char* pReuslt = m_pData + ( m_firstBlock * m_blockSize );
		m_firstBlock = *(long*) pReuslt;

		-- m_availSize;

		*(long*)pReuslt = m_blockSize - 4 ;
				
		return pReuslt+4;
	}

	bool   Release(void* pData)
	{   
		char*  plist = (char*)pData-4;

		*(long*)plist = m_firstBlock;

		m_firstBlock = ( plist - m_pData ) / m_blockSize;
		
		++ m_availSize;
		return true;
	}

	bool   Check(void* pData)
	{
		if ( !pData || pData < m_pData ) return false;
		char*  plist = (char*)pData-4;
		if ( (plist-m_pData)% m_blockSize != 0 || (plist-m_pData)/ m_blockSize > m_Length )
		{
			//std::cout <<" release mem failed !" << std::endl;
			return false;
		}
		return true;
	}

	bool   Full()
	{
		return m_availSize == m_Length;
	}

	void   print()
	{
		std::cout <<"\tblocksize    "  << m_blockSize - 4 
				  <<" m_firstBlock "   << m_firstBlock
				  <<" m_availSize  "   << m_availSize << std::endl;
	}

private:
	///  字段大小
	int			m_blockSize;
	char*		m_pData;
	long		m_firstBlock;
	long		m_availSize;
	long		m_Length;
};