

#pragma once 

#include "RasDial.h"
#include "SendToMail.h"


namespace tp_ipc_peer_namespace
{
	class ctpool;
}
//class  tp_ipc_peer_namespace::ctpool;						   

class  SendMailApp
{

public:
	SendMailApp();

	~SendMailApp();

	void  Start();

	bool  IsSendMail();

private:

	/// ²¦ºÅ
	MyRasDial*						m_RasDial;
	/// Ïß³Ì³Ø
	tp_ipc_peer_namespace::ctpool*  m_TPool;
};