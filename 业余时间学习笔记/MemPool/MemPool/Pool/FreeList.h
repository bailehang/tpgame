
#pragma  once 

#define  MaxSize  ((1<<15)-1)
class Chunk
{

	friend class SmallObjAllocator;
public:

	Chunk( int blocksize, long  len )
	{
		ReleaseAll();

		if( MaxSize < len )
		{
			len = MaxSize;
		}
		m_blockSize = blocksize;
		m_availSize = len;
		m_Length  = len;
		m_firstBlock = 0;
		m_pData = new  char [ blocksize * len ];

		char* p = m_pData;
		for ( short i = 0 ; i < len ; p+=blocksize)
		{
			*(short*) p = ++i;
		}

		//std::cout <<" 分配信息 ";
		//print();
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

	bool   Release(void* pData)
	{   
		char*  plist = (char*)pData;
		*(short*)plist = m_firstBlock;
		m_firstBlock = ( plist - m_pData ) / m_blockSize;

		++ m_availSize;
		//print();
		return true;
	}

	bool   Check(void* pData)
	{
		if ( !pData || pData < m_pData ) return false;
		char*  plist = (char*)pData;
		if ( (plist-m_pData)% m_blockSize != 0 || (plist-m_pData)/ m_blockSize > m_Length )
		{
			//std::cout <<" release mem failed !" << std::endl;
			return false;
		}
		return true;
	}

	void   print()
	{
		std::cout <<" blocksize  " << m_blockSize 
				  <<" m_firstBlock "<< m_firstBlock
				  <<" m_availSize "<< m_availSize << std::endl;
	}

private:
	///  字段大小
	int   m_blockSize;
	char* m_pData;
	short m_firstBlock;
	short m_availSize;
	short m_Length;
};