/*============================================
*
* file:   DataBlockSetWriteRead.h
*
* brief:  有固定大小的内存，动态分配数据块的基本机构
*
* Authtor:Expter
*
* Data:   20090616
*
* Modity: //20090818
===========================================*/

#pragma once

typedef unsigned char BYTE;

class CDataBlockPool
{
public:
	CDataBlockPool(long MaxSize);

	long m_lTestFlag;
public:
	virtual ~CDataBlockPool(void);

    /// user define method
public:
	BYTE* Base()      { return m_pBase;}

	long  GetCurSize() { return m_nCurSize; }
	void  SetCurSize(long lSize);

	long  GetMaxSize() { return m_nMaxSize; }


	/// user define val
private:
	BYTE * m_pBase;      /// 数据起始地址
	long   m_nCurSize;   /// 当前数据块大小
	long   m_nMaxSize;   /// 总数据大小


	/// user define private method
private:
	/// 屏蔽这些操作
	CDataBlockPool &operator = (const CDataBlockPool &);
	CDataBlockPool(const CDataBlockPool&);

};
