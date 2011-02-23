// Design.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/*
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
*/

#include <list>
#include <iostream>

using namespace  std;

template < typename type>
class Base
{
public:
	Base( type tp ) 
	{ m_type = tp;}

	class Handle
	{
	public:
		Handle()
		{
			Base<type>::m_handlelist.push_back( this );
		}
		virtual void todo( Base<type> * base)
		{
			std::cout <<" Handle base " << std::endl;
		}
	};

public:
	virtual void   todo()
	{
		for( std::list<Handle*>::iterator itr = m_handlelist.begin() ; itr != m_handlelist.end() ; itr ++ )
		{
			(*itr)->todo(this);
		}  
	}


protected:
	static  type            		m_type;
	static  std::list<Handle*>  	m_handlelist;
};

template < typename type > 
class BaseObj1 : public Base< type >
{
public:
	BaseObj1( type tp ) : Base ( tp )
	{		}

	void print()
	{
		std::cout << " BaseObj1 print() " << std::endl;
		todo();
	}	
};

template < typename type >
class BaseObj2 : public Base < type >
{
public:
	BaseObj2( type tp ) : Base ( tp )
	{		}

	void display()
	{
		//todo();
	}
};

template < typename type >
class Print : public Base< type >::Handle
{
public:
	virtual	void todo( Base<type> * base )
	{
		std::cout << " 	baseobj1  m_type address " << std::endl;
	}
};

template < typename type >
class display : public Base < type >::Handle 
{
public: 
	virtual void todo( BaseObj2<type> * base )
	{
		std::cout <<" baseobj2 m_type address " << std::endl;
	}
};	





template < typename type >
std::list<typename Base<type>::Handle*>		 Base<type>::m_handlelist;

template < typename type >
type Base<type>::m_type;

// template < typename type >
// std::list<typename BaseObj1<type>::Handle*>  BaseObj1<type>::m_handlelist;
// template < typename type >
// std::list<typename BaseObj2<type>::Handle*>  BaseObj2<type>::m_handlelist;

int _tmain(int argc, _TCHAR* argv[])
{
	BaseObj1<int>  b1(1);
	Print<int>     p1;

	b1.todo();

	return 0;
}

