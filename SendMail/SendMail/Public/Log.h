
#pragma  once 

#include <fstream>
#include <string>

using namespace std;

class  CLog
{
public:
	CLog(const char* filename):out(filename,ios_base::app|ios_base::out)
	{

	}

	template< typename type >
	CLog& operator  << (const type& v)
	{
		 out << v ;

		 return *this;
	}

	~CLog()
	{
		out.close();
	}

private:

	ofstream out;

};