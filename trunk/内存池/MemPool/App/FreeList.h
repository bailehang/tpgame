/** 
 *  @breif	用基于freelist的单链表实现内存块
 *	@author	expter
 *  @date   2010.05.01
 */

#pragma  once 

#include <assert.h>
#include <windows.h>
#include <WinBase.h>

#include "Allocator.h"

#define  BLOCKSIZE( size )   ( size + LONGSIZE )

class  List
{				
public:
	/// @param blocksize 每块大小
	List( long blocksize, long  count);

	~List()	;

	void*   Allocator();

	bool    Free(void *pData);

	bool	Check(void* pData) const ;

	bool    Empty() const {
		return  m_validsize == m_blockcount;					
	}

	bool    Full()  const {
		return m_validsize <= 0;
	}

private:

	void    Init()	;

	void    ReleaseAll()  ;
	
private:   
	char*	m_pData;		///< point new data
	long    m_firstblock;	///< first block
	long    m_nodesize;		///< node  size
	long    m_blockcount;	///< block len
	long    m_validsize;	///< valid size
	
};
