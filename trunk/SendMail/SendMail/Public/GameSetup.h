
#pragma  once 

class  CGameSetup
{

public:
	CGameSetup();
	~CGameSetup();

	void  Load();

	void  LoadSetUp();
	
private:
	long  LoadSendAddr(std::string str);

	void  LoadRecvAddr(std::string Recvfile);

	void  LoadContext();

	void  LoadSubTxt();

	void  LoadRole();

	void  LoadSendAddrList();

	void  LoadRecvAddrList();

	void  LoadSmtpList();
};