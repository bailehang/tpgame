

#pragma  once 

#include <string>
#include <list>

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

class  SendToMail
{

public:
	SendToMail(tagSend msend,tagSendInfo msendinfo):m_Send(msend),m_SendInfo(msendinfo)
	{
		m_list.clear();
	}

	void  AppendUser(std::list<std::string>& List)
	{
		m_list = List;
	}

	bool  Send();


public:

	/// 当前发送账号信息
	tagSend			m_Send;
	tagSendInfo		m_SendInfo;

	/// 发送联系人
	std::list<std::string>  m_list;

};