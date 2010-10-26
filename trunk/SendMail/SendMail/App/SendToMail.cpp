#include "stdafx.h"
#include "SendToMail.h"
#include "../Public/Singleton.h"

#import "../mail.dll"

bool  SendToMail::Send( size_t index )
{

	size_t tsize = GetInstObj(MailLoginInfo).m_Vec.size();
	if ( tsize <= m_SendID )
	{
		return false ;
	}				  
	tagSend		m_Send     = GetInstObj(MailLoginInfo).m_Vec[ m_SendID ].m_Send;
	tagSendInfo m_SendInfo = GetInstObj(MailLoginInfo).m_Vec[ m_SendID ].m_SendInfo;

	CoInitialize(NULL); 	// COM的初始化

	try{

		jmail::IMessagePtr pMessage("JMail.Message");

		// 发件人邮箱
		pMessage->From = m_Send.login.c_str();

		// 发件人姓名
		pMessage->FromName = m_Send.name.c_str() ;

		// 添加收件人1, 无收件人姓名与PGP KEY

		for ( std::list<std::string>::iterator itr = m_list.begin() ; itr != m_list.end() ; itr ++ )
		{
			 pMessage->AddRecipient((*itr).c_str(),"朋友","");	
		}
		
		pMessage->MailServerUserName=m_Send.user.c_str();

		pMessage->MailServerPassWord=m_Send.pword.c_str();

		//pMessage->PutServerPort(465);

		// 优先级设置,1-5逐次降低, 3为中级
		pMessage->Priority = 3;

		pMessage->Charset = "GB2312";

		pMessage->Subject = m_SendInfo.subject.c_str();

		pMessage->ContentType =  m_SendInfo.Formt ? "text/html " : "text/plain ";

		// 正文
		pMessage->Body = m_SendInfo.Context.c_str();	

		pMessage->Send(m_Send.stmp.c_str(),VARIANT_FALSE);
		//pMessage->Send("smtp.qq.com",VARIANT_FALSE);
	}
	catch(_com_error e)
	{
		CString strerr;
		strerr.Format("错误号:%0x\r\n 错误信息:%s\r\n错误描述是%s",e.Error(),(LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description());
	    return false;
	}
	CoUninitialize();	
	return true;
}

/*
//pMessage->AddNativeHeader("Date", "2010-05-10");

//pMessage->AddNativeHeader("From", "tangpeng@126.com");

//pMessage->AddHeader("Mailer", "Expter 1.0");		  	// 邮件发送器名称
//pMessage->AddNativeHeader("Disposition-Notification-To", "52311940@qq.com");

// 添加附件
//pMessage->AddAttachment("C:\\CONFIG.SYS", VARIANT_FALSE, "application/octet-stream");
//pMessage->AddAttachment("C:\\test.gif", VARIANT_TRUE, "image/gif");

*/