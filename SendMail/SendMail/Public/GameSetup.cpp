#include "stdafx.h"			  
#include "GameSetup.h"
#include "Singleton.h"
#include "BaseDef.h"

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
	LoadSetUp();


	LoadSendAddr();
	LoadRecvAddr();
	LoadContext();
	LoadRole();
}

void CGameSetup::LoadSendAddr()
{
	tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);

	ifstream file(GlobalSetup.SendAddrList.c_str());

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}

	std::vector<tagSend>& SendAddr = GetInstObj(MailLoginInfo).m_Vec;

	char addr[100],pword[100],smtp[100];
	char user[100];
	while ( !file.eof() )
	{
		file >> addr >> user >> pword >> smtp; 

		tagSend  base(addr,user,user,pword,smtp);

		SendAddr.push_back( base );
	}

	file.close();
}

void CGameSetup::LoadRecvAddr()
{
	tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);

	ifstream file(GlobalSetup.RecvAddrList.c_str());

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}

	std::list<std::string>& RsSendList = GetInstObj(DestList).m_SendList;

	char addr[100],tmp[100];

	while ( !file.eof() )
	{
		file >> addr >> tmp; 

		RsSendList.push_back( addr );
	}
	file.close();
}

void  CGameSetup::LoadContext()
{
	tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);
	ifstream file( GlobalSetup.MailContext.c_str() );

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}

	tagSendInfo& SendInfo = GetInstObj(tagSendInfo);

	char str[512];
	
	/// ∂¡»°–≈œ¢
	while ( !file.eof() )
	{
		file.getline( str , 512 , '\n' );

		if ( str[0] == '#' )
		{
			stringstream  out;
			out << str;

			byte  cbyte;
			out >>	cbyte >> SendInfo.Formt >> SendInfo.file >> SendInfo.fileaddr;
			break;
		}			 
	}

	while ( !file.eof() )
	{
		file.getline( str, 512,'\n');
		SendInfo.Context +=str;
	}
	file.close();
}

void  CGameSetup::LoadSetUp()
{
	ifstream file("Config\\Setup.ini");

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}

	tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);
	
	char  str[64];
	while ( !file.eof() )
	{
		file >> str >> GlobalSetup.BroadName
			>>  str >> GlobalSetup.Account
			>>  str >> GlobalSetup.PassWord
			>>  str >> GlobalSetup.ThreadNum
			>>  str >> GlobalSetup.SendInter
			>>  str >> GlobalSetup.BoTime
			>>  str >> GlobalSetup.SendNum
			>>  str >> GlobalSetup.SendMax
			>>  str >> GlobalSetup.SameDomainMax
			>>  str >> GlobalSetup.SendAddrList
			>>  str >> GlobalSetup.RecvAddrList
			>>  str >> GlobalSetup.SendRole
			>>  str >> GlobalSetup.MailContext
			>>  str >> GlobalSetup.MailSub;
	}

	file.close();
}

void   CGameSetup::LoadRole()
{
	tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);
	ifstream file( GlobalSetup.SendRole.c_str() );

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}

	tagSendRole&  tMailSub = GetInstObj(tagSendRole);


	char  str[64];
	file.getline( str , 512 , '\n' );

	stringstream  out;
	out << str;

	byte  cbyte;
	out >>	cbyte >> tMailSub.NoChangeRand >> tMailSub.Rand1 >> tMailSub.Rand2 >> tMailSub.Rand3;

	while ( !file.eof() )
	{
		file >> str ;
		tMailSub.sReplace.push_back( str );
	}
	
	file.close();
}


void   CGameSetup::LoadSubTxt()
{
	tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);
	ifstream file( GlobalSetup.MailSub.c_str() );

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}
 
	tagMailSub&  tMailSub = GetInstObj(tagMailSub);


	while ( !file.eof() )
	{
		MailSubNode  Node;
		file >> Node.sMailSub >> Node.lReNum;

		tMailSub.VecMailSub.push_back( Node );
	}

	file.close();
}
