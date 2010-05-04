
#include <iostream>
using namespace std;

class A
{
};

class B : public A
{
};

class C
{
};

/// ÅÐ¶ÏDevÊÇ·ñ¼Ì³ÐBase
template < class Dev, class Base>
class IsHerit
{

 public:
     static  int  test( Base* )
     {
	 return 0;
     }
     static  char test( void* )
     {
	 return 0;
     }

     enum { Result = ( sizeof( t(Base*)NULL) == sizeof( t(Dev*)NULL) ),};
};


int main()
{

	bool ret1 = IsHerit<B,A>::Result;// true
	bool ret2 = IsHerit<C,A>::Result;// false
	bool ret3 = IsHerit<A,B>::Result;// false
	return 0;
}
