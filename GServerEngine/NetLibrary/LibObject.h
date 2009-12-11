#pragma once

#include "ConnectionManager.h"
#include "PacketManager.h"
#include "..\Public\log\LogManager.h"

#define LOG	CLibObject::Log.Write

class CLibObject 
{
public: 	
	static	CConnectionManager		ConnectionManager;
	static  CPacketManager			PacketManager;
	static  CLogManager				Log;
};

