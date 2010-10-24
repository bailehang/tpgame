
#pragma once 

class  RasDial
{

public:
	RasDial();
	RasDial(CString user,CString pword,CString name);
	~RasDial();

	bool  Listen();
	void  Stop();

private:

	HRASCONN hRasConn;
	CString  m_User;
	CString  m_PassWord;


	CString  m_Name;

};