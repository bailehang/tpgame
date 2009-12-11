#pragma once

#include "..\Public\TypeDef.h"
#include "..\NetLibrary\Server.h"
//#include "CacheWorker.h"

class CClientMsg :
	public CServer
{
public:
	CClientMsg(void);
	~CClientMsg(void);
public:

	void	Init();

	void	AccedeProcess( CConnection *pCon , CMessage *pPacket);

	bool	ConnectCacheServer(int  Count , char* ip,int port);

	void	run();

private:
	CConnection  *m_CServerCon[200];			/// ¿Í»§¶Ë

	static   long   CountNum ;
};

