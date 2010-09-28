
#pragma once 

struct sock4req1 
{ 
	char VN; 
	char CD; 
	unsigned short Port; 
	unsigned long IPAddr; 
	char other[1]; 
}; 

struct sock4ans1 
{ 
	char VN; 
	char CD; 
}; 

struct sock5req1 
{ 
	char Ver; 
	char nMethods; 
	char Methods[255]; 
}; 

struct sock5ans1 
{ 
	char Ver; 
	char Method; 
}; 

struct sock5req2 
{ 
	char Ver; 
	char Cmd; 
	char Rsv; 
	char Atyp; 
	char other[1]; 
}; 

struct sock5ans2 
{ 
	char Ver; 
	char Rep; 
	char Rsv; 
	char Atyp; 
	char other[1]; 
}; 

struct authreq 
{ 
	char Ver; 
	char Ulen; 
	char Name[255]; 
	char PLen; 
	char Pass[255]; 
}; 

struct authans 
{ 
	char Ver; 
	char Status; 
};