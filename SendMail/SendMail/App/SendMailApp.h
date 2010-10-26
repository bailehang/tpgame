

#pragma once 

#include "RasDial.h"
#include "Thread.h"
#include "SendToMail.h"


class  SendMailApp
{

public:
	SendMailApp();

	~SendMailApp();

	void  Start();

	bool  IsSendMail();

private:

	/// 拨号
	MyRasDial*						m_RasDial;
	/// 线程池
	tp_ipc_peer_namespace::ctpool*  m_TPool;
	/// 一个临界区类
	sync::csectionlock			    m_Sendlock_;
	/// 一个临界区类
	sync::csectionlock			    m_SendTolock_;
	

};