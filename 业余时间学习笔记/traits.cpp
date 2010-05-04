#include <iostream>
using namespace std;

template < bool user_param = true>
class  cfun
{	
	public:
		cfun()
		{
			std::cout <<" cfun bool user_param = ture "<< std::endl;
		}
};

template <>
class cfun<false>
{
	public:
		cfun()
		{
			std::cout <<" cfun<false> class " << std::endl;
		}
};

int main()
{
	cfun  c;
        std::cout<<" main function <<std::endl;
	return 0;
}
