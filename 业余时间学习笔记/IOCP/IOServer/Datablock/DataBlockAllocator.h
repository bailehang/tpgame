
/*============================================
*
* file:   DataBlockAllocator.h
*
* brief:  固定大小内存快速分配器
*
* Authtor:Expter
*
* Data:   20090616
*
* Mend:   //20090918
===========================================*/



#pragma once

#include <list>
#include <map>
#include <windows.h>
using namespace std;

class CDataBlockPool;


/// 一个数据块的详细信息
struct tagDBOpParam
{
	tagDBOpParam()
	{
		memset(this,0,sizeof(tagDBOpParam));
	}
	/// 当前读写数据块编号
	long  nCurNum;
	/// 当前读写数据块的大小
	long  nCurDBSize;
	/// 当前读写数据的位置
	long  nCurPos;
	/// 当前读写数据块的指针
	BYTE* pDBPtr;

};

class CDataBlockAllocator
{
public:
	CDataBlockAllocator(void);
	CDataBlockAllocator(int nDBNum,int nDBSize,int bIniShareDB = false );

public:
	~CDataBlockAllocator(void);


private:
	typedef list<CDataBlockPool*>  FreeDataBlocks;
	typedef list<CDataBlockPool*>::iterator itFreeDB;

	FreeDataBlocks				   m_FreeDataBlocks;
	CRITICAL_SECTION			   m_CSFreeDB;
	int							   m_nMaxDBNum;
	int							   m_nDBSize;

	/// 用在逻辑层临时使用的数据块
	/// 个数不多，使用较大的数据块来满足各种大小的需求

	typedef multimap<long,BYTE*>  MMapShareDB;
	typedef pair <long,BYTE*>     SHareDBPair;
	typedef multimap<long,BYTE*>::iterator itShareDB;
	MMapShareDB					  m_ShareDataBlocks;

	typedef map<BYTE*,long>       mapAllocedRecord;
	typedef map<BYTE*,long>::iterator itAllocR;
	mapAllocedRecord              m_AllocedShareDB;


	/// 允许的最大共享数据块个数
	map<long,long> m_MapTest;

private:
	void Initial(int bIniShareDB = false);
	void Release();

public:
	CDataBlockPool* AllocDB(long lTestFlag = 0);
	void FreeDB(CDataBlockPool *);

	BYTE * AllockShareDb(long lSize);
	void FreeShareDB(BYTE* pData);

	void PutAllocInfo(const char * pszFileName);

};
