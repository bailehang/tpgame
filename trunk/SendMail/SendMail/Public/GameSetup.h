
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

	void  LoadRecvAddr();

	void  LoadContext();

	void  LoadSubTxt();

	void  LoadRole();
};