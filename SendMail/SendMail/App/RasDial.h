
#pragma once 

class  MyRasDial
{

public:
	MyRasDial();
	MyRasDial(CString user,CString pword,CString name);
	~MyRasDial(){}

	bool  Listen();
	void  Stop();

private:

	HRASCONN hRasConn;
	CString  m_User;
	CString  m_PassWord;


	CString  m_Name;

};