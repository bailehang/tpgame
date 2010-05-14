
#pragma  once 

class Chunk
{

public:
	Chunk( int blocksize, long  len )
	{
		blocksize = blocksize;
		m_availSize = len;
		m_firstBlock = 0;
		m_pData = new  char [ blocksize * len ];

		char* p = m_pData;
		for ( short i = 0 ; i < len ; p+=blocksize)
		{
			*(short*) p = ++i;
		}
	}

	~Chunk()
	{
		ReleaseAll();
	}

	void ReleaseAll()
	{
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
		m_firstBlock = *(short*) pReuslt;

		-- m_availSize;
		return pReuslt;
	}

	void   Release(void* pData)
	{
		if ( !pData || pData < m_pData ) return ;
		
		char*  plist = (char*)pData;
		if ( (plist-m_pData)% m_blockSize != 0)
		{
			std::cout <<" release mem failed !" << std::endl;
		}
		*(short*)plist = m_firstBlock;

		m_firstBlock = ( plist - pData ) / m_blockSize;

		++ m_availSize;
	}

	void   print()
	{
		std::cout <<" blocksize " << m_blockSize ;
	}

private:
	///  ×Ö¶Î´óÐ¡
	int   m_blockSize;
	char* m_pData;
	short m_firstBlock;
	short m_availSize;
};