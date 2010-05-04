
#pragma  once 

enum  eState
{
	ent_Flee =0,
	ent_Peace,

	ent_Num
};

class State
{
public:
	// Enter a new State
	virtual void  Enter(CBaseEntity*)= 0;

	// Exectue
	virtual void  Exectue(CBaseEntity* )=0;

	// Exit
	virtual void  Exit(CBaseEntity* )=0;

	virtual	~State();

	eState		  GetState()	{	return  -1 ;	}
};

class CStateReg
{
	State**		g_StateList;
public:
	CStateReg()
	{
		g_StateList = new State *[ent_Num];

		g_StateList[ent_Flee] = new CFleeState();
		g_StateList[ent_Peace]= new CPeaceState();
	}

	~CStateReg()
	{
		if (  g_StateList != NULL)
		{
			delete [] g_StateList;
		}
		g_StateList = NULL;
	}

	State*		GetState( eState  e)
	{
		return g_StateList[ e ];
	}
};

