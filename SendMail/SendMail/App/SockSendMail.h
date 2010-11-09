

#pragma  once 

#include <list>
#include <vector>
#include <map>
#include "../Public/log.h"

static CLog logFail("log\\Fail.txt");
static CLog logSuce("log\\suce.txt");

class  SocketSendToMail
{

public:
	SocketSendToMail(size_t size )
	{
		m_SendID = size;
	}

	void  AppendUser(std::list<std::string>& List)
	{
		m_list = List;
	}
	 
	bool  operator() ( )
	{
		return Send( m_SendID );
	}

private:
	bool  Send( size_t index );


private:
	bool  CreateSocket()
	{
		try
		{
			return m_Socket.Create() > 0 ? true : false ;
		}
		catch (...)
		{
			long eid = GetLastError();
			logFail <<" Create Fail Errid= " << eid  <<"\r\n";
			return false;
		}
		
	}

	void  ReleaseSocket()
	{
		m_Socket.Close();
	}

	bool  CheckResponse(std::string RecvCode,char* Buf )
	{
		try
		{
			m_Socket.Receive( Buf, 1024 );

			if ( RecvCode[0] == Buf[0] &&
				RecvCode[1] == Buf[1] &&
				RecvCode[2] == Buf[2] )
			{
				return true;
			}

			logFail <<	Buf ;
			return false;
		}
		catch(...)
		{
			return false;
		}
	}

	bool  CheckAccount(std::string ip,std::string name,std::string pass);

	/// 发送到雅虎
	bool  SendYahoo(std::string& tmp,std::string & context);

	bool  SendOther(std::string& tmp,std::string & context);

public:

	/// 联系ID 
	size_t		    m_SendID;
	/// 套接字
	CSocket			m_Socket;
	/// 发送联系人
	std::list<std::string>  m_list;
};