

#pragma  once 

#include <list>
#include <string>

void  ListCopy(list<std::string>::iterator begin,
			   list<std::string>::iterator end,
			   list<std::string>::iterator& dest,long Count)
{
	for ( ; Count-- && begin != end ;)
	{
		*dest ++ = *begin++;
	}
}