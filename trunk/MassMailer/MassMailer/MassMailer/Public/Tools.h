
#pragma  once 

#include <string>
#include <map>
#include <vector>
#include <fstream>

using namespace std;


struct   tagSmtp
{
	tagSmtp(long p,string do):port(p),domain(do)
	{

	}
	long	port;
	string  domain;
};

struct	 tagProxy
{
	tagProxy(long p,string t):port(p),type(t)
	{

	}

	string  type;
	long	port;
};

struct   tagSendAddr
{
	string  name;
	string  password;
	string  smtp;
};