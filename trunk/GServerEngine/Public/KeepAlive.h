
/*
*	File  :
*   Brief :  定义一个数据包的存活时间
*   Author:  Expter 
*	Creat Date:  [2009/11/2]
*/

#pragma   once

#include "Array.h"
#include "BaseNode.h"
#include "..\NetLibrary\Packet.h"

class  CKeepAlive : public CArray 
{

public:

	CKeepAlive()
	{
		bUse	=	false;
		KeepAlivePacket.SetMsgPtcl( CLIENT_KEEPALIVE);
		//KeepAlivePacket.Size = sizeof( CPacket );
	}

	/*
	*	初始化数据包的什么周期
	*/
	bool InitKeepAlive( bool QUse , unsigned long Timer , unsigned long nowtime)
	{
		bUse = QUse;

		NowTime			= nowtime;
		KeepAliveTimer	= Timer;
		NextKeepAliveTime = nowtime + KeepAliveTimer;

		return true;
	}

	unsigned long   GetNowTime()		{	return NowTime ; }

	unsigned long	GetKeepAliveTime()	{	return (NowTime - KeepAliveTimer) ; }

	unsigned long	GetAllowedKeepAliveTime() {	return NowTime - 60000 ; }
	
	CPacket *		GetKeepAlivePacket(){	return & KeepAlivePacket ;	}

	bool			KeepAlive(  unsigned long nowtime )
	{
		if ( bUse )
		{
			NowTime		= nowtime;

			if ( NextKeepAliveTime <= nowtime )
			{
				NextKeepAliveTime = nowtime + KeepAliveTimer ;

				return true;
			}
		}

		return false;
	}

private:

	unsigned long   KeepAliveTimer;
	unsigned long   NowTime;

	unsigned long   NextKeepAliveTime;

	bool			bUse;
	CMessage		KeepAlivePacket;
	

};