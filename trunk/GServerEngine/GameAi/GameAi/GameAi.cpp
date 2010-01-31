// GameAi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{

	GetInst(CStateSet).RegCStateSet( Ent_PeaceState , "CPeaceState");
	GetInst(CStateSet).RegCStateSet( Ent_FleeState  , "CFleeState");
	GetInst(CStateSet).RegCStateSet( Ent_FightState , "CFightState");
	GetInst(CStateSet).RegCStateSet( Ent_FollowState, "CFollowState");
	GetInst(CStateSet).RegCStateSet( Ent_DeadState  , "CDeadState");
	GetInst(CStateSet).RegCStateSet( Ent_GoHomeState, "CGoHomeState");

	CHumanEntity		human1(Ent_Player);
	CHumanEntity		human2(Ent_Player);
	CHumanEntity		human3(Ent_Player);

	CMonsterEntity		Monster1(Ent_Monster);
	CMonsterEntity		Monster2(Ent_Monster);
	CMonsterEntity		Monster3(Ent_Monster);

	GetInst(CEntityManager).RegEntityManager( &human1 );
	GetInst(CEntityManager).RegEntityManager( &human2 );
	GetInst(CEntityManager).RegEntityManager( &human3 );

	GetInst(CEntityManager).RegEntityManager( &Monster1 );
	GetInst(CEntityManager).RegEntityManager( &Monster2 );
	GetInst(CEntityManager).RegEntityManager( &Monster3 );
	
	while ( 1 )
	{
		// 处理所有的ai
		GetInst(CEntityManager).Update();
		// 处理所有的事件
		GetInst(CEventDispatch).DispacthEevent();

		Sleep( 100 );
	}
	return 0;
}

