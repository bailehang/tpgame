/*============================================
*
* file:   DataBlockSetWriteRead.h
*
* brief:  内存块读写
*
* Authtor:Expter
*
* Data:    20090616
*
* Modty:   20090818
===========================================*/


#pragma once

#include <vector>
#include "DataBlockPool.h"
#include "DataBlockAllocator.h"
using namespace std;


class  CDataBlockPool;
struct tagDBOpParam;
class  CDataBlockAllocator;


/// 写数据块
class CDataBlockSetWrite
{


public:
	typedef vector<CDataBlockPool*>::iterator itDataBlocks;

	vector<CDataBlockPool*>*			      pDataSet;
	/// 数据块分配器
	CDataBlockAllocator*					  pDBAllocator;
	/// 当前数据块集合操作集合
	tagDBOpParam*							  pDBWriteParam;
	unsigned long *							  pToltalSize;

	//CDataBlockAllocator						  *pDBAllocator;

public:
	CDataBlockSetWrite()
	{
		pDataSet      = NULL;
		pDBAllocator  = new  CDataBlockAllocator(1024,2048,false);
		pDBWriteParam = NULL;
		pToltalSize   = NULL;
	}

	~CDataBlockSetWrite()
	{
	}

	void Initial(CDataBlockAllocator *pAllocator,
		tagDBOpParam         *pWritePam,
		vector<CDataBlockPool*>* pSet,
		unsigned long *		 pSize)
	{
		pDataSet      = pSet;
		pDBAllocator  = pAllocator;
		pDBWriteParam = pWritePam;
		pToltalSize   = pSize;
	}
public:
	/// 添加数据
	void AddtoByteArray(void * pSource, long n)
	{
		AddBuf((BYTE*)pSource,n);
	}
	void AddtoByteArray(const char * pStr)
	{
		long size = lstrlenA(pStr);
		AddBuf((BYTE*)&size,sizeof(long));
		AddBuf((BYTE*)pStr,strlen(pStr));
	}
	template<typename obj>
	void AddtoByteArray(obj   l)
	{
		AddBuf((BYTE*)&l,sizeof(obj));
	}

	void PutAllInfo()
	{
		pDBAllocator->PutAllocInfo("111.txt");
	}
private:
	void AddBuf(BYTE * pBuf,long Size)
	{
		long lTempSize = Size;
		while(pDBWriteParam->pDBPtr != NULL && Size > 0 )
		{
			int lMin = min(Size,pDBWriteParam->nCurDBSize-pDBWriteParam->nCurPos);
			memcpy(pDBWriteParam->pDBPtr+pDBWriteParam->nCurPos,pBuf,lMin);
			pDBWriteParam->nCurPos +=lMin;
			(*pDataSet)[pDBWriteParam->nCurNum]->SetCurSize(lMin);

			Size -=lMin;
			pBuf +=lMin;
			if(pDBWriteParam->nCurPos >= pDBWriteParam->nCurDBSize && Size >0)
			{
				AddWriteDataBlock();
			}
		}
		(*pToltalSize) +=lTempSize;
	}
	void AddWriteDataBlock()
	{
		pDBWriteParam->nCurNum ++;
		pDBWriteParam->nCurPos = 0;
		pDBWriteParam->nCurDBSize=0;
		pDBWriteParam->pDBPtr = NULL;


		CDataBlockPool  *pB  = pDBAllocator->AllocDB(2000);
		if(pB == NULL)
		{
			printf(" AddWriteDataBlock Flag failed ！");
			return ;
		}

		(*pDataSet).push_back(pB);
		pDBWriteParam->nCurDBSize = (*pDataSet)[pDBWriteParam->nCurNum]->GetMaxSize();
		pDBWriteParam->pDBPtr     = (*pDataSet)[pDBWriteParam->nCurNum]->Base();
	}

public:
	inline void DBWriteSet(tagDBOpParam  &pWritePam , CDataBlockPool *pool ,vector<CDataBlockPool*> &DataSet )
	{
		pool->SetCurSize(2048);
		DataSet.push_back(pool);

		pWritePam.nCurDBSize = 2048;
		pWritePam.nCurNum    = 0;
		pWritePam.nCurPos    = 0;
		pWritePam.pDBPtr     = pool->Base();

		unsigned long                     total = 0;
		Initial(pDBAllocator,&pWritePam,&DataSet,&total);
		return;
	}	


};





/// 读数据块
class DataBlockSetRead
{

public:
	typedef vector<CDataBlockPool*>::iterator itDataBlocks;
	vector<CDataBlockPool*>*				  pDataSet;
	/// 当前数据块集合操作集合
	tagDBOpParam*							  pDBReadParam;

	/// 构造函数
public:
	DataBlockSetRead()
	{
		pDataSet = NULL;
		pDBReadParam = NULL;
	}
	/// 初始化
	void Inital(tagDBOpParam * pReadParam,vector<CDataBlockPool*>* pSet)
	{
		pDataSet  = pSet;
		pDBReadParam = pReadParam;
	}
public:
	void *GetBufferFromByteArray(void *Buf,long lLen)
	{
		GetBuf((BYTE*)Buf,lLen);
	}

	char *GetBufferFromByteArray(char *Buf,long lLen)
	{
		if(lLen <= 1) return NULL;
		long len = GetObjDataFromByteArray<long>();
		if(len < 0 )  len =0;

		len = min(len,lLen-1);
		GetBuf((BYTE*)Buf,len);
		Buf[len] ='\0';
		return Buf;
	}

	template<typename type>
	inline type GetObjDataFromByteArray()
	{
		type t =0;
		GetBuf((BYTE*)&t,sizeof(type));
		return t;
	}

	/// inline 函数
private:
	void * GetBuf(BYTE * buf, long size)
	{
		while( pDBReadParam->pDBPtr != NULL && size >0 )
		{
			int MinVal = min(size,pDBReadParam->nCurDBSize - pDBReadParam->nCurPos);
			memcpy(buf,pDBReadParam->pDBPtr+pDBReadParam->nCurPos,MinVal);

			size -=MinVal;
			pDBReadParam->nCurPos +=MinVal;
			buf +=MinVal;

			if(pDBReadParam->nCurPos >= pDBReadParam->nCurDBSize)
				ReadDataBlock();
		}
		return buf;
	}
	void ReadDataBlock()
	{
		pDBReadParam->nCurNum ++;
		pDBReadParam->nCurPos = 0;
		pDBReadParam->nCurDBSize=0;
		pDBReadParam->pDBPtr = NULL;

		if( pDBReadParam->nCurNum < (int)pDataSet->size() )
		{
			pDBReadParam->nCurDBSize =(*pDataSet)[pDBReadParam->nCurNum]->GetCurSize();
			pDBReadParam->pDBPtr     =(*pDataSet)[pDBReadParam->nCurNum]->Base();
		}
	}

public:
	/// 读取数据块
	inline void DBReadSet(tagDBOpParam	& pWritePam ,CDataBlockPool *pool ,vector<CDataBlockPool*> &DataSet )
	{
		pWritePam.nCurDBSize = 2048;
		pWritePam.nCurNum    = 0;
		pWritePam.nCurPos    = 0;
		pWritePam.pDBPtr     = pool->Base();

		Inital(&pWritePam,&DataSet);
	}

};


