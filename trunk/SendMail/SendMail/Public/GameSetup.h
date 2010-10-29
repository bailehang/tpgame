
#pragma  once 

class  CGameSetup
{

public:
	CGameSetup();
	~CGameSetup();

	void  Load();
	
private:
	void  LoadSendAddr();

	void  LoadRecvAddr();

	void  LoadContext();

	void  CreateNum();
};