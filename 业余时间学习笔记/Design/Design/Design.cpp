// Design.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

描述:

virtual  Base * getBase() ;

/// 根据obj指针,获得具体子类具体信息，访问之类特有操作
void  function(Base  *obj)
{
	/// 由于obj只是Base基类指针

	BaseObj1 * obj1;
	BaseObj2 * obj2;

	if ( obj1 = dynamic_cast<BaseObj1*>( obj )  )
	{
		/// to do
	}
	else if ( obj2 = dynamic_cast<BaseObj2*>( obj ) )
	{
		/// to do
	}
	else 
	{
		/// ... you get the idea
	}
}

问题描述：当增加Base子类型时，上面的if else语句会变得越来越长。

解决上面问题可以通过以下几种方式来解决:

1. 一般情况下考虑多态，obj只需要调用虚函数即可。即：
     void function( Base  *obj )
	{
		obj->todo();
	}

	但是如果function函数的某个操作是free obj时，通过todo恐怕不合适。


2. 考虑到上面问题，我们利用visitor模式来
	那么同时得提供下面的操作接口:
	virtual void visit( BaseObj1 * obj1 );
	virtual void visit( BaseObj1 * obj2 );

	/// 如果有多个子类型的话，需要重写visit，此时fuction
	void function( Base  *obj )
	{
		visit( obj );
	}

3. 一个新解决方式.




int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

