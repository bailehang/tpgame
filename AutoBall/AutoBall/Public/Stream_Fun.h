#pragma once 

#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include "Log.h"

using namespace std;

template <class T>
inline std::string ttos(const T& t, int precision = 2)
{
	std::ostringstream buffer;

	buffer << std::fixed << std::setprecision(precision) << t;

	return buffer.str();
}

inline std::string btos(bool b)
{
	if (b) return "true";
	return "false";
}

template <typename T>
inline T GetValueFromStream(std::ifstream& stream)
{
	T val;

	stream >> val;
	if (!stream)
	{
		throw std::runtime_error("Attempting to retrieve wrong type from stream");
	}

	return val;
}

template <typename T>
void WriteBitsToStream(std::ostream& stream, const T& val)
{
	int iNumBits = sizeof(T) * 8;

	while (--iNumBits >= 0)
	{
		if ((iNumBits+1) % 8 == 0) stream << " ";
		unsigned long mask = 1 << iNumBits;
		if (val & mask) stream << "1";
		else stream << "0";
	}
}


inline bool  isRead(ifstream& stream,const char* key)
{
	string str;
	stream >> str;

	while( str.c_str() != key) 
	{
		if ( stream.eof() )
		{
			return  false;
		}
		stream >> str;
	}
	return true;
}

inline bool  isWrite(ofstream& stream,const char* key)
{
	stream << key;
	return true;
}

inline void PrintLuaMsg( const char* pInfo )
{
	char  str[128];

	sprintf_s<128>( str , "%s" , pInfo );

	CLog log( "cs.txt" );
	log << str ;

}