

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
	std::string  smtp;		/// smtp

	tagSend( string login_,string name_,string user_,string pword_,string smtp_)
	{
		login = login_;
		name  = name_;
		user  = user_;
		pword = pword_;
		smtp  = smtp_;
	}
};

struct  tagSendInfo
{
	byte		 Formt;		/// 格式，是否是ＨＴＭＬ
	byte		 file;
	std::string  fileaddr;  
	std::string  Context;   /// 正文内容
};

struct  tagMailBase
{							   	
	/// 当前发送账号信息
	tagSend			m_Send;
};

struct  MailLoginInfo
{
	std::vector<tagSend>  m_Vec;
	long				  Num1;
	long				  Num2;
};

struct  DestList
{
	/// 发送列表
	std::list<std::string>    m_SendListOne;
	std::list<std::string>    m_SendListTwo;
};

struct  tagGlobalSetup
{
	/// 全局配置信息
	
	/// 基本配置
	long         ThreadNum;
	long		 SendInter;
	long		 BoTime;
	long		 SendNum;
	long		 SendMax;
	long		 SameDomainMax;

	/// 宽带拨号
	std::string   BroadName;
	std::string   Account;
	std::string   PassWord;

	/// 配置信息
	std::string   SendAddrList;
	std::string	  RecvAddrList;
	std::string   SendRole;
	std::string	  MailContext;
	std::string   MailSub;
	std::string   SmtpIni;
	
};

struct   MailSubNode
{
	std::string   sMailSub;
	long		  lReNum;
};

struct   tagMailSub 
{
	vector<MailSubNode>  VecMailSub;
};

struct   tagSendRole
{
	long		 NoChangeRand;	///> 维持原状不改变标题概率
	long		 Rand1;			///> 随机1个字符串概率
	long		 Rand2;			///> 随机2个字符串概率
	long		 Rand3;			///> 随机3个字符串概率

	vector<std::string>  sReplace;
};

struct	tagSendAddrList
{
	std::string			SendList;
	std::vector<string>	KeyValue;
};

struct	SendAddrList
{
	 tagSendAddrList	SendListOne;
	 tagSendAddrList	SendListTwo; 
};

struct	tagSmtpList 
{
	long    value;
	string  index;
	string  smtp;
};

struct	SmtpList 
{
	std::vector<tagSmtpList>	tSmtpList;
};