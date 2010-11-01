#include "stdafx.h"			  
#include "GameSetup.h"
#include "Singleton.h"
#include "../App/SendToMail.h"

#include <fstream>
#include <iostream>
using namespace std;

CGameSetup::CGameSetup()
{
	
}

CGameSetup::~CGameSetup()
{
	
}

void CGameSetup::Load()
{
	LoadSendAddr();
	LoadRecvAddr();
	LoadContext();
	//CreateNum();
}

void CGameSetup::LoadSendAddr()
{
	ifstream file("SendAddr.ini");

	if ( !file.is_open() || file.eof() )
	{
		return ;
	}

	std::vector<tagSend>& SendAddr = GetInstObj(MailLoginInfo).m_Vec;

	char addr[100],pword[100],smtp[100];

	while ( !file.eof() )
	{
		file >> addr >> pword >> smtp; 

		char user[100];

		char* p = strstr(addr,"@");
		long  len = p-addr;
		strncpy( user,addr,len);
		user[len] ='\0';

		tagSend  base(addr,user,user,pword,smtp);

		SendAddr.push_back( base );
	}
}

void CGameSetup::LoadRecvAddr()
{
	ifstream file("RecvAddr.ini");

	if ( !file.is_open() || file.eof() )
	{
		return ;
	}

	std::list<std::string>& RsSendList = GetInstObj(DestList).m_SendList;

	char addr[100],tmp[100];

	while ( !file.eof() )
	{
		file >> addr >> tmp; 

		RsSendList.push_back( addr );
	}
}

void  CGameSetup::LoadContext()
{
	ifstream file("Context.txt");

	if ( !file.is_open() || file.eof() )
	{
		return ;
	}

	tagSendInfo& SendInfo = GetInstObj(tagSendInfo);

	char str[512];
	SendInfo.Formt = 1;
	SendInfo.subject ="ÄãºÃ,friends!";

	while ( !file.eof() )
	{
		file.getline( str, 512,'\n');
		SendInfo.Context +=str;
	}
}


void  CGameSetup::CreateNum()
{
   ofstream out("txt.txt");


   long  BaseValue = 20000001;
   for ( int i = 0; i < 100 ; i++ )
   {
	   out << BaseValue + rand() <<"@qq.com\t" << " aaa " << std::endl;
   }

   out.close();
}