/*
 * Brief :  一个CGUID类，通过win32的com直接生成，这里主要参考某些实现写的一个直接CUID一些扩展函数
 * Author:  Expter  
 * Date  :  [2009-12-10]
 * 参考星尘传说CGUID的实现，使用该类 必须先调用Initialize初始COM   ，重载了=、==。     
 **/

#pragma warning(disable:4996)
#pragma once

#include <hash_map>
#include <string>

using namespace stdext;

class CGUID
{
private:
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];

	friend class hash_guid_compare;

public:
	static void  Initialize();
	static void  Uninitialize();

	static bool  CreateGUID(CGUID &guid);
	static CGUID GUID_INVALID;

	CGUID(void)
	{
		memset(this, 0, sizeof(CGUID));
	}

	explicit CGUID(const char* strGuid);
	~CGUID(void)
	{
	}

	CGUID & operator = (const CGUID & guid)
	{
		if( &guid != this )
			memcpy(this, &guid, sizeof(CGUID));
		return *this;
	}

	inline bool operator == (const CGUID & guid) const
	{
		return (((unsigned long *) this)[0] == ((unsigned long *) &guid)[0] &&
			   ((unsigned long *) this)[1]  == ((unsigned long *) &guid)[1] &&
			   ((unsigned long *) this)[2]  == ((unsigned long *) &guid)[2] &&
			   ((unsigned long *) this)[3]  == ((unsigned long *) &guid)[3] );
	}

	bool operator != (const CGUID & guid) const
	{
		return !(operator ==(guid));
	}

	bool operator < (const CGUID& guid) const
	{
		if( ((unsigned long *)this)[0] < ((unsigned long *)&guid)[0] )
			return true;
		else if( ((unsigned long *)this)[0] == ((unsigned long *)&guid)[0] )
		{
			if(( (unsigned long *)this)[1] < ((unsigned long *)&guid)[1] )
				return true;
			else if( ((unsigned long *)this)[1] == ((unsigned long *)&guid)[1])
			{
				if( ((unsigned long *)this)[2] < ((unsigned long *)&guid)[2] )
					return true;
				else if( ((unsigned long *)this)[2] == ((unsigned long *)&guid)[2])
					if( ((unsigned long *)this)[3] < ((unsigned long *)&guid)[3] )
						return true;
			}
		}
		return false;
	}

	bool CopyConstrucutureCalFunc(const char* szValue, long& retValue);
	bool tostring(char * strGuid) const
	{
		if(strGuid == 0 )	return false;
		sprintf(strGuid,"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		Data1, Data2, Data3, Data4[0],Data4[1],
		Data4[2],Data4[3],Data4[4],Data4[5],Data4[6],Data4[7]);
		return true;
	}
	bool IsInvalided()
	{
		if( memcmp(this, 0, sizeof(CGUID)) )
			return false;
		return true;
	}
};


// 提供给hash使用
class guid_compare
{
public:
	inline bool operator( )( const CGUID& Key1, const CGUID& Key2 ) const
	{
		if(Key1 < Key2)
			return true;
		return false;
	}

};

class hash_guid_compare:public hash_compare<CGUID,guid_compare>
{
private:
	guid_compare	comp;
public:
	//计算散列值
	size_t operator( )( const CGUID& Key ) const
	{
		return ((unsigned long *) &Key)[0] + ((unsigned long *) &Key)[1] +
			((unsigned long *) &Key)[2] + ((unsigned long *) &Key)[3];
	}
	//less operator
	bool operator( )( const CGUID& _Key1,const CGUID& _Key2 ) const
	{
		return comp(_Key1,_Key2);
	}
};

const CGUID NULL_GUID = CGUID::GUID_INVALID;
