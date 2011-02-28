#include <iostream>
#include <string>
using namespace std;

template < typename  T>
struct  Type2Type
{
	typedef  T  type;
};

class Widget
{
	public:
		Widget(string str,int n)
		{
			std::cout << str.c_str() <<" " << n << std::endl;
		}
};

template < typename U , typename T >
T* Create(const U& arg, Type2Type<T> )
{
	std::cout << " arg "<< arg << std::endl;
	return new T( arg );
}

template < typename U >
Widget* Create(const U& arg,Type2Type<Widget>)
{
	return new Widget(arg,-1);
}


int main()
{
	string* p = Create("string",Type2Type<string>());
	getchar();
	return 0;
}
