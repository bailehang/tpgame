

#pragma  once 

#include <string>
#include <list>
#include <vector>

using namespace std;

struct  tagSend
{
	std::string  login;		/// 账号
	std::string  name;		/// 名字
	std::string  user;		/// 登录user
	std::string  pword;		/// 登录密码
	std::string  stmp;		/// smtp
};

struct  tagSendInfo
{
	byte		 Formt;		/// 格式，是否是ＨＴＭＬ
	std::string  subject;   /// 主题
	std::string  Context;   /// 正文内容
};

struct  tagMailBase
{							   	
	/// 当前发送账号信息
	tagSend			m_Send;
	tagSendInfo		m_SendInfo;
};

struct   MailLoginInfo
{
public:
	std::vector<tagMailBase>  m_Vec;
};

struct  DestList
{
	std::list<std::string>    m_SendList;
};

class  SendToMail
{

public:
	SendToMail(size_t size )
	{
		m_list.clear();
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

public:

	/// 联系ID 
	size_t		    m_SendID;
	/// 发送联系人
	std::list<std::string>  m_list;

};