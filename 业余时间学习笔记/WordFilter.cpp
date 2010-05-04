// WordFilter.cpp : 定义控制台应用程序的入口点。
//

/*
   File :  WordFilter.cpp 
   brief:  关键字过滤程序，复杂度为O(n)，线性
   Author: Expter
   Data  : 2009/06/30

   对汉字或者字符进行哈希算法，先转换为unicode编码，然后求其hash值。

   主要算法为:
   1.建立2个过滤关键字数组:数组1：为单个字符  数组2：为2个或者多个字符
   2.求出数组1，2的hash值，数组2的hash值只求出前2个字符的hash值即可。
   3.扫描待检测的文本，然后每次取2个字符，查找数组2是否有匹配，如果没有则查找数组1。。。。  查找为O(1)


   不足:
   不能很好的分词。过滤不是很准确，每次只能1，2个词的过滤。
*/
#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <windows.h>
#include <wchar.h>  
#include <iosfwd>
using namespace std;

wchar_t des1 [5][2] = { L"汉",L"字",L"测",L"试",L"个"};
wchar_t des2 [3][5] = { L"用汉", L"的啥" ,L"测试啊"};
wchar_t src[]  = { L"这个原来是打算的啥子东西用汉字只是一个是不是测试"};


/// 求汉字的哈希值
long HashFun(wchar_t  word)
{
	BYTE l = LOBYTE(word);
	int  h = HIBYTE(word);

	long num = h << 8 ;
	num +=l;
	return num;
}

long HashFun(wchar_t * word)
{
	return HashFun(word[0]) + HashFun(word[1]);
}


void  ParamVer(map<long,int> hashmp , wchar_t *src , int i)
{
	long val = HashFun(src[i+1]);
	if(hashmp[val] == 1)
	{
		src[i+1] = L'*';
	}
}
void  VmAlorgthm(map<long,int> hashmp,wchar_t *src)
{
	long val = 0;
	int  m = wcslen(src) ;
	// O(n);
	for(int i = 0 ; i < m-1  ; i ++)
	{
		if( HashFun(src[i]) != L'*')
		{
			val = HashFun(src[i]) + HashFun(src[i+1]);
			if( hashmp[val] == 1)
			{
				src[i] = L'*';
				src[i+1] =L'*';
			}
			else
			{
				val = HashFun(src[i]);
				if(hashmp[val] == 1)
				{
					src[i] = L'*';
				}
				else
				{
					ParamVer(hashmp,src,i);
				}
			}
		}
		else
		{
			ParamVer(hashmp,src,i+1);
		}
	}
	ParamVer(hashmp,src,m-1);
}

template<class T1, class T2>
void ReadBufFile( T2 t2,const char * filename)
{
	T1 t1(filename);
	if (t1.fail())
	{
		return ;
	}
	string str;
	while( !t1.eof() )
	{
		t1 >> str;
		t1.push_back(str);
	}
	t1.close();
}



void Alorithm2( )
{

}


int _tmain(int argc, _TCHAR* argv[])
{
	wcout.imbue(locale("chs"));     
	typedef map<long,int> HASHMAP;

	cout <<" 需要过滤文本: ";
	wcout<< src <<endl;
	cout <<" 过滤关键字 : " ;
	for(int i = 0 ;i < 5; i++)
		wcout << des1[i][0] <<" ";
	wcout <<endl;
	cout <<" 过滤关键词 : " ;
	for(int i = 0 ;i < 3; i++)
		wcout << des2[i] <<" ";
	wcout <<endl;
   
    vector<string>  vword;
	//ReadBufFile<ifstream,vector<string> >(vword,"1.txt"); 
	long  val = 0;

	HASHMAP hash_map;
	/// 字 hash
	for(int i = 0 ; i < 5 ; i++)
	{
		val = HashFun(des1[i][0]);
		hash_map[val] = 1;
	}
	/// 词 hash
	for(int i =0 ; i < 3 ; i++)
	{
		val = HashFun(des2[i]);
		hash_map[val] = 1;
	}
	
	VmAlorgthm(hash_map,src);
	
	cout <<"\n-------------------------------------------------------------\n"
		<<" 过滤后的文本: ";
	wcout<< src <<endl;

	return 0;
}


