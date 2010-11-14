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
	LoadSmtpList();

	LoadSendAddrList();

//	LoadSendAddr();
//	LoadRecvAddr();
	LoadContext();
	LoadRole();
	LoadSubTxt();
	
}

long CGameSetup::LoadSendAddr(std::string str)
{
	ifstream file(str.c_str());

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return 0;
	}

	long num = 0;
	std::vector<tagSend>& SendAddr = GetInstObj(MailLoginInfo).m_Vec;
	SmtpList& SmtpIni = GetInstObj(SmtpList);
	string addr,pword,smtp;
	string user;
	while ( !file.eof() )
	{
		// user smtp
		file >> addr >> pword; 
	
		size_t inex = -1;
		for ( size_t i = 0 ; i < SmtpIni.tSmtpList.size() ; i++ )
		{
			if ( addr.find( SmtpIni.tSmtpList[i].index ) != string::npos )
			{
				inex = i ;
				break;
			}
		}
		if ( inex < 0 )
			continue;
		if ( SmtpIni.tSmtpList[inex].value == 0 )
		{
			int len  = addr.find("@");
			user = addr.substr(0,len);
		}
		else
			user  = addr;

		smtp = SmtpIni.tSmtpList[inex].smtp ;

		tagSend  base(addr,user,user,pword,smtp);

		SendAddr.push_back( base );
		num ++;
	}

	file.close();
	return num;
}

void CGameSetup::LoadRecvAddr(std::string Recvfile)
{
	//tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);

	ifstream file(Recvfile.c_str());

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}

	std::vector<string>&    AddrList    = GetInstObj(SendAddrList).SendListOne.KeyValue;
	std::list<std::string>& RsSendList1 = GetInstObj(DestList).m_SendListOne;
	std::list<std::string>& RsSendList2 = GetInstObj(DestList).m_SendListTwo;
	DestList& dlist = GetInstObj(DestList);

	char addr[100];

	while ( !file.eof() )
	{
		file >> addr ; 

		string stmp = addr;
		bool  flag = false;
		for( size_t i = 0 ; i < AddrList.size() ; i++ )
		{
			if( stmp.find( AddrList[i] ) != string::npos )
			{
				flag = true;
				break;
			}
		}
		//if( flag )
		RsSendList1.push_back( stmp );
		//else
		//	RsSendList2.push_back( stmp );
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
	
	/// ¶ÁÈ¡ÐÅÏ¢
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
			>>  str >> GlobalSetup.MailSub
			>>	str	>> GlobalSetup.SmtpIni;
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

void  CGameSetup::LoadSendAddrList()
{
	tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);

	ifstream file(GlobalSetup.SendAddrList.c_str());

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}
	
	SendAddrList&  AddrList = GetInstObj(SendAddrList);
	MailLoginInfo& LoginInfo = GetInstObj(MailLoginInfo);

	byte   bbyte;
	long   num ;
	char   str[256];
	if( !file.eof() )
	{
		file >> bbyte >> AddrList.SendListOne.SendList;
		file >> num;

		LoginInfo.Num1 = LoadSendAddr( AddrList.SendListOne.SendList );
		for ( int i = 0 ; i < num ; i++ )
		{
			file >> str;
			AddrList.SendListOne.KeyValue.push_back( str );
		}
		
		file >> bbyte >> AddrList.SendListTwo.SendList;
		file >> num;

		LoginInfo.Num2 = LoadSendAddr( AddrList.SendListTwo.SendList );
		for ( int i = 0 ; i < num ; i++ )
		{
			file >> str;
			AddrList.SendListTwo.KeyValue.push_back( str );
		}
	}

	file.close();

	LoadRecvAddrList();
}

void  CGameSetup::LoadRecvAddrList()
{
	tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);

	ifstream file(GlobalSetup.RecvAddrList.c_str());

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}

	char   str[256];
	std::set<std::string> fileSet;
	while( !file.eof() )
	{
		file.getline( str , 256 ,'\n' );
		fileSet.insert( str );
	}

	for ( std::set<std::string>::iterator it=fileSet.begin() ; it != fileSet.end() ; it++ )
	{
		LoadRecvAddr( *it );
	}

	file.close();
}


void   CGameSetup::LoadSmtpList()
{
	tagGlobalSetup& GlobalSetup = GetInstObj(tagGlobalSetup);
	SmtpList& SmtpIni = GetInstObj(SmtpList);
	ifstream file(GlobalSetup.SmtpIni.c_str());

	if ( !file.is_open() || file.eof() )
	{
		file.close();
		return ;
	}

	while ( !file.eof() )
	{
		tagSmtpList	 smtp;

		file >> smtp.index >> smtp.value >> smtp.smtp;

		SmtpIni.tSmtpList.push_back( smtp );
	}										

	file.close();
}