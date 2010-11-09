
#pragma  once 

class  CGameSetup
{

public:
	CGameSetup();
	~CGameSetup();

	void  Load();

	void  LoadSetUp();
	
private:
	void  LoadSendAddr();

	void  LoadRecvAddr(std::string Recvfile);

	void  LoadContext();

	void  LoadSubTxt();

	void  LoadRole();

	void  LoadSendAddrList();

	void  LoadRecvAddrList();
};