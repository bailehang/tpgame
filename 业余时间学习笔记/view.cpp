// view.cpp : 定义控制台应用程序的入口点。
//

/// 错误
/// 1.溢出
/// 2.fun不是静态函数，有默认的this参数
#include "stdafx.h"
#include <iostream>
using namespace std;

int  (* fun)(int * p );

struct  tagStr 
{
	int  spfun( int  size,void (*fun)(int* p) , int* p)
	{
		return spfun( size, fun , p );
	}
	
	void  fun(int *p)
	{
		static  int arr[10000];
		int     arr2[10000];
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	tagStr   tag;
	tag.spfun( 20000, tag.fun, NULL) ;
	return 0;
}

