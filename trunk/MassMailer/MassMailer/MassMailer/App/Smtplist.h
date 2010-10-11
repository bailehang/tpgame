
#pragma once 

#include "../Public/Tools.h"


template < typename type, template<typename> class table> 
class  Property
{

private:

};

class  SmtpList
{
	typedef map<string,tagSmtp>	 MpSmtp;

private:

	MpSmtp     m_smtp;

	void       Load()
	{
		m_smtp.clear();

		ifstream  in("smtp.ini");

		if ( in.eof())
		{
			return;
		}

		string  type,smtp;
		long    port;
		while ( !in.eof() )
		{
			in >> type >> smtp >> port;

			m_smtp[ type ] = tagSmtp(port,smtp);
		}
	}
public:

	SmtpList()
	{
		Load();
	}

	bool  GetSmtp( string type , tagSmtp & smtp) const 
	{
		MpSmtp::iterator  itr = m_smtp.find( type );

		if ( itr != m_smtp.end() )
		{
			smtp =  itr->second;
			return true;
		}

		return false;
	}

};