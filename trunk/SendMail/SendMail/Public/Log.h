
#pragma  once 

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

class  CLog
{
public:
	CLog(const char* filename):file(filename)
	{

	}

	template< typename type >
	CLog& operator  << (const type& v)
	{

		ofstream out;
		out.open( file.c_str(), ios_base::app|ios_base::out);

		out << v ;

		out.close();

		return *this;
	}

private:

	std::string file;

};