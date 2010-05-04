#include <stdio.h>
#include <hash_map>
#include <string>
#include <iostream>
using namespace std;
using namespace stdext;
struct str_hash_Algorihtm
{
	size_t GetVal( const string  & str) const
	{
		unsigned long Val = 0;
		for( size_t i = 0 ; i < str.size() ; i++ )
		{
			Val += str[i]+1;
		}
		return Val;
	}
	inline bool operator ()( const string & str1 , const string & str2)const
	{
		if ( GetVal(str1) < GetVal(str2) )
			return true;
		return false;
	}
};
struct str_equal:public hash_compare<string,str_hash_Algorihtm>
{ 	
	str_hash_Algorihtm cmp;
	bool operator () ( const string & str1 , const string & str2)const 
	{
		return cmp(str1,str2);
	}
};
int main()
{
	hash_map< string , string , str_equal> ht1;
	return 0;
} 