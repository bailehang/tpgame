
#pragma once 

#pragma warning(disable:4786) 
#include <string>
using namespace std;

class CBase
{

public:

	/**
	 *  Encode Data Stream
	 */
	string Encode(const unsigned char* Data,int DataByte);

	/**
	 *  Decode Data Stream
	 */
	string Decode(const char* Data,int DataByte,int& OutByte);
};