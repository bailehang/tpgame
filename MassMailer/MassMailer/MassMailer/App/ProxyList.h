
#pragma once 

#include "../Public/Tools.h"

class  CProxyList
{
	typedef map<string,tagProxy>	 MpProxy;
private:

	MpProxy		m_proxy;

	void Load()
	{
		m_proxy.clear();

		ifstream  in("Proxy.ini");

		if ( in.eof())
		{
			return;
		}

		string  type,ip;
		long    port;
		while ( !in.eof() )
		{
			in >> ip >> port>> type ;

			m_smtp[ type ] = tagProxy(port,ip);
		}
	}

public:
	CProxyList()
	{
		Load();
	}

	bool  GetProxy( string ip , tagProxy & proxy) const 
	{
		MpProxy::iterator  itr = m_proxy.find( ip );

		if ( itr != m_smtp.end() )
		{
			proxy =  itr->second;
			return true;
		}

		return false;
	}

};