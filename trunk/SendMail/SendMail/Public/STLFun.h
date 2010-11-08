

#pragma  once 

#include "Log.h"
#include <list>
#include <string>

inline void  ListCopy(list<std::string>::iterator& begin,
			   list<std::string>::iterator end,
			   list<std::string>& dest,long Count)
{
	for ( ; Count-- && begin != end ;)
	{
		dest.push_back( *begin++ );
	}
}

inline void  WriteFile(const char* file,const char* pStr)
{
	CLog log(file);
	log << pStr <<"\n";
}