#include "Stdafx.h"
#include "State.h"
#include "StateMachine.h"	 
#include "../Entity/PlayerBase.h"
#include "../Entity/GoalKeeper.h"		  
#include "../Goal.h"
#include "../SoccerTeam.h"
#include "../SoccerPitch.h"
#include "../Messageing/MessageDispatcher.h"
#include "../SteeringBehaviors.h"
#include "../../Public/MsgImpl.h"
#include "../../Config.h"		

//uncomment to send state info to debug window
//#define GOALY_STATE_INFO_ON


EmptyFun(void,GlobalKeeperState,Enter,GoalKeeper);
EmptyFun(void,GlobalKeeperState,Execute,GoalKeeper);
EmptyFun(void,GlobalKeeperState,Exit,GoalKeeper);

bool GlobalKeeperState::OnMessage(GoalKeeper* keeper, const tagMessage& telegram)
{
	switch(telegram.Msg)
	{
	case Msg_GoHome:
		{
			keeper->SetDefaultHomeRegion();

			keeper->GetFSM()->ChangeState(&GetInstObj(ReturnHome));
		}

		break;

	case Msg_ReceiveBall:
		{
			keeper->GetFSM()->ChangeState(&GetInstObj(InterceptBall));
		}

		break;

	}//end switch

	return false;
}


EmptyMsg(bool,TendGoal,OnMessage,GoalKeeper);

void TendGoal::Enter(GoalKeeper* keeper)
{
	//激活
	keeper->Steering()->InterposeOn(GetInstObj(CGameSetup).GoalKeeperTendingDistance);

	/// interpose将使智能体处在球和目标之间，该调用设置目标
	keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());
}

void TendGoal::Execute(GoalKeeper* keeper)
{
	/// 随着球的位置的改变，后方的插入目标位置也将改变，所以必须在每个更新步骤更新
	keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());

	/// 如果球进入范围，守门员抓住他，然后改变状态把球传回到赛场中
	if (keeper->BallWithinKeeperRange())
	{
		keeper->Ball()->Trap();

		keeper->Pitch()->SetGoalKeeperHasBall(true);

		keeper->GetFSM()->ChangeState(&GetInstObj(PutBallBackInPlay));

		return;
	}

	/// 如果球在预定义的距离，守门员已到那个位置尽力截住它
	if (keeper->BallWithinRangeForIntercept() && !keeper->Team()->InControl())
	{
		keeper->GetFSM()->ChangeState(&GetInstObj(InterceptBall));
	}

	/// 如果守门员离球门线太远了，而且没有对方队员的威胁，他应该移回球门
	if (keeper->TooFarFromGoalMouth() && keeper->Team()->InControl())
	{
		keeper->GetFSM()->ChangeState(&GetInstObj(ReturnHome));

		return;
	}
}


void TendGoal::Exit(GoalKeeper* keeper)
{
	keeper->Steering()->InterposeOff();
}

EmptyMsg(bool,ReturnHome,OnMessage,GoalKeeper);

void ReturnHome::Enter(GoalKeeper* keeper)
{
	keeper->Steering()->ArriveOn();
}

void ReturnHome::Execute(GoalKeeper* keeper)
{
	keeper->Steering()->SetTarget(keeper->HomeRegion()->Center());

	/// 如果离初始区域足够近，或者对手控制了球，改变状态守门
	if (keeper->InHomeRegion() || !keeper->Team()->InControl())
	{
		keeper->GetFSM()->ChangeState(&GetInstObj(TendGoal));
	}
}

void ReturnHome::Exit(GoalKeeper* keeper)
{
	keeper->Steering()->ArriveOff();
}


EmptyMsg(bool,InterceptBall,OnMessage,GoalKeeper);

void InterceptBall::Enter(GoalKeeper* keeper)
{
	keeper->Steering()->PursuitOn();  

#ifdef GOALY_STATE_INFO_ON
	debug_con << "Goaly " << keeper->ID() << " enters InterceptBall" <<  "";
#endif
}

void InterceptBall::Execute(GoalKeeper* keeper)
{ 
	///	 如果守门员离球门很远，那么他应该移会自己的区域，除非他是离球最近的球员
	///  那样的情况下，它应该尽量拦截住球
	if (keeper->TooFarFromGoalMouth() && !keeper->isClosestPlayerOnPitchToBall())
	{
		keeper->GetFSM()->ChangeState(&GetInstObj(ReturnHome));

		return;
	}

	/// 如果球在守门员手可触及的范围，应该抓住球，然后在把他传回赛场
	if (keeper->BallWithinKeeperRange())
	{
		keeper->Ball()->Trap();

		keeper->Pitch()->SetGoalKeeperHasBall(true);

		keeper->GetFSM()->ChangeState(&GetInstObj(PutBallBackInPlay));

		return;
	}
}

void InterceptBall::Exit(GoalKeeper* keeper)
{
	keeper->Steering()->PursuitOff();
}


EmptyFun(void,PutBallBackInPlay,Exit,GoalKeeper);
EmptyMsg(bool,PutBallBackInPlay,OnMessage,GoalKeeper);

void PutBallBackInPlay::Enter(GoalKeeper* keeper)
{
	//let the team know that the keeper is in control
	keeper->Team()->SetControllingPlayer(keeper);

	//send all the players 回到初始位置
	keeper->Team()->Opponents()->ReturnAllFieldPlayersToHome();
	keeper->Team()->ReturnAllFieldPlayersToHome();
}


void PutBallBackInPlay::Execute(GoalKeeper* keeper)
{
	PlayerBase*  receiver = NULL;
	Vector2D     BallTarget;

	/// 测试是否有队员在更前场的位置，这样我们可能可以传球给他，如果这样，那么传球
	//test if there are players further forward on the field we might
	//be able to pass to. If so, make a pass.
	if (keeper->Team()->FindPass(keeper,
		receiver,
		BallTarget,
		GetInstObj(CGameSetup).MaxPassingForce,
		GetInstObj(CGameSetup).GoalkeeperMinPassDist))
	{     
		//make the pass   
		keeper->Ball()->Kick(Vec2DNormalize(BallTarget - keeper->Ball()->Pos()),
			GetInstObj(CGameSetup).MaxPassingForce,keeper);

		/// 守门员不在控球
		//goalkeeper no longer has ball 
		keeper->Pitch()->SetGoalKeeperHasBall(false);

		/// 让接球队员知道球正传过来
		//let the receiving player know the ball's comin' at him
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			keeper->GetID(),
			receiver->GetID(),
			Msg_ReceiveBall,
			&BallTarget);

		/// 回到守门状态
		//go back to tending the goal   
		keeper->GetFSM()->ChangeState(&GetInstObj(TendGoal));

		return;
	}  

	keeper->SetVelocity(Vector2D());
}

