#include "Stdafx.h"
#include "State.h"
#include "StateMachine.h"
#include "../Entity/PlayerBase.h"
#include "../Entity/FieldPlayer.h"	  
#include "../Goal.h"
#include "../SoccerTeam.h"
#include "../SoccerPitch.h"
#include "../Messageing/MessageDispatcher.h"
#include "../SteeringBehaviors.h"
#include "../../Public/MsgImpl.h"
#include "../../Config.h"	


#define PLAYER_STATE_INFO_ON

EmptyFun(void,GlobalPlayerState,Enter,FieldPlayer);
EmptyFun(void,GlobalPlayerState,Exit,FieldPlayer);

void GlobalPlayerState::Execute(FieldPlayer* player)                                     
{
	if( !player->Team()->IsChaseBall() ) return;

	/// 如果球员占有并接近球，那么降低他的最大速度
	if((player->BallWithinReceivingRange()) && (player->isControllingPlayer()))
	{
		player->SetMaxSpeed(GetInstObj(CGameSetup).PlayerMaxSpeedWithBall);
	}
	else
	{
		player->SetMaxSpeed(GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall);
	}

}

bool GlobalPlayerState::OnMessage(FieldPlayer* player, const tagMessage& telegram)
{
	switch(telegram.Msg)
	{
	case Msg_ReceiveBall:
		{
			//set the target
			player->Steering()->SetTarget(*(static_cast<Vector2D*>(telegram.ExtraInfo)));

			//change state 
			player->GetFSM()->ChangeState(&GetInstObj(ReceiveBall));

			return true;
		}

		break;

	case Msg_SupportAttacker:
		{
			//如果已经在接应，立即返回
			if (player->GetFSM()->isInState(GetInstObj(SupportAttacker)))
			{
				return true;
			}

			/// 设置目标最佳接应位置
			//set the target to be the best supporting position
			player->Steering()->SetTarget(player->Team()->GetSupportSpot());

			//change the state
			player->GetFSM()->ChangeState(&GetInstObj(SupportAttacker));

			return true;
		}

		break;

	case Msg_Wait:
		{
			//change the state
			player->GetFSM()->ChangeState(&GetInstObj(Wait));

			return true;
		}

		break;

	case Msg_GoHome:
		{
			player->SetDefaultHomeRegion();

			player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion));

			return true;
		}

		break;

	case Msg_PassToMe:
		{  
			/// 得到请求传球的队员位置
			//get the position of the player requesting the pass 
			FieldPlayer* receiver = static_cast<FieldPlayer*>(telegram.ExtraInfo);

#ifdef PLAYER_STATE_INFO_ON
			// debug_con << "Player " << player->GetID() << " received request from " <<
			//              receiver->ID() << " to make pass" << "";
#endif

			/// 如果球不在球员可触及的范围，或请求队员不具备射门的条件，改队员不能传球给请求队员
			if (player->Team()->Receiver() != NULL ||
				!player->BallWithinKickingRange() )
			{
#ifdef PLAYER_STATE_INFO_ON
				//debug_con << "Player " << player->GetID() << " cannot make requested pass <cannot kick ball>" << "";
#endif

				return true;
			}

			//make the pass   传球
			player->Ball()->Kick(receiver->Pos() - player->Ball()->Pos(),
				GetInstObj(CGameSetup).MaxPassingForce,player);


#ifdef PLAYER_STATE_INFO_ON
			//debug_con << "Player " << player->GetID() << " Passed ball to requesting player" << "";
#endif

			///通知接球队员，开始传球
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
				player->GetID(),
				receiver->GetID(),
				Msg_ReceiveBall,
				&receiver->Pos());



			//change state   
			player->GetFSM()->ChangeState(&GetInstObj(Wait));

			player->FindSupport();

			return true;
		}

		break;

	}//end switch

	return false;
}

EmptyMsg(bool,ChaseBall,OnMessage,FieldPlayer);

void ChaseBall::Enter(FieldPlayer* player)
{
	if( !player->Team()->IsChaseBall() ) return;
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 ChaseBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2  )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 ChaseBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}

	player->Steering()->SeekOn();

#ifdef PLAYER_STATE_INFO_ON
	// debug_con << "Player " << player->GetID() << " enters chase state" << "";
#endif
}

void ChaseBall::Execute(FieldPlayer* player)                                     
{
	/// 如果球在他能踢到 ，那么该队员改变状态为KickBall		  BallWithinReceivingRange
	if (player->BallWithinKickingRange())
	{
		player->GetFSM()->ChangeState(&GetInstObj(KickBall));

		return;
	}
	
	/// 如果改球队员离球最近，继续追球
	if (player->isClosestTeamMemberToBall())
	{
		player->Steering()->SetTarget(player->Ball()->Pos());

		return;
	}

	/// 如果该队员不是离球最近，那么他应该回到自己的初始位置，等待下一次
	player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion));
}


void ChaseBall::Exit(FieldPlayer* player)
{
	player->Steering()->SeekOff();
}


void SupportAttacker::Enter(FieldPlayer* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 SupportAttacker x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 SupportAttacker x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	player->Steering()->ArriveOn();

	player->Steering()->SetTarget(player->Team()->GetSupportSpot());

#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters support state" << "";
#endif
}

EmptyMsg(bool,SupportAttacker,OnMessage,FieldPlayer);

void SupportAttacker::Execute(FieldPlayer* player)                                     
{
	if( !player->Team()->IsChaseBall() ) return;

	/// 如果自己球队丢失对球的控制权，球员回到起始区域
	if (!player->Team()->InControl())
	{
		player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion)); return;
	} 

	/// 如果最佳接应点改变了，那么改变操控目标
	if (player->Team()->GetSupportSpot() != player->Steering()->Target())
	{    
		player->Steering()->SetTarget(player->Team()->GetSupportSpot());

		player->Steering()->ArriveOn();
	}

	/// 如果这么球员可以射门，切进攻队员可以把球传给他，那么把球传给该队员
	if( player->Team()->CanShoot(player->Pos(),
		GetInstObj(CGameSetup).MaxShootingForce))
	{
		player->Team()->RequestPass(player);
	}

	/// 如果这名队员在接应点，且他的球队仍控制球，他应该停在那儿，转向面对着球
	if (player->AtTarget())
	{
		player->Steering()->ArriveOff();

		/// 队员跟踪球
		player->TrackBall();

		player->SetVelocity(Vector2D(0,0));

		/// 如果没有收到其他队员的威胁，那么请求传球
		if (!player->isThreatened())
		{
			player->Team()->RequestPass(player);
		}
	}
}


void SupportAttacker::Exit(FieldPlayer* player)
{
	player->Team()->SetSupportingPlayer(NULL);

	player->Steering()->ArriveOff();
}




void ReturnToHomeRegion::Enter(FieldPlayer* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 ReturnToHomeRegion x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 ReturnToHomeRegion x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	player->Steering()->ArriveOn();

	if (!player->InsideHomeRegion(player->Steering()->Target(), Region::halfsize))
	{
		player->Steering()->SetTarget(player->GetHomeCenter());
	}

#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters ReturnToHome state" << "";
#endif
}


EmptyMsg(bool,ReturnToHomeRegion,OnMessage,FieldPlayer);

void ReturnToHomeRegion::Execute(FieldPlayer* player)
{
	if( !player->Team()->IsChaseBall() ) return;

	if (player->Pitch()->GameOn())
	{
		//if the ball is nearer this player than any other team member  &&
		//there is not an assigned receiver && the goalkeeper does not gave
		//the ball, go chase it
		if ( player->isClosestTeamMemberToBall() &&
			player->Team()->IsReceiver() &&
			!player->Pitch()->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

			return;
		}
	}

	//if game is on and close enough to home, change state to wait and set the 
	//player target to his current position.(so that if he gets jostled out of 
	//position he can move back to it)
	if (player->Pitch()->GameOn() && player->HomeRegion()->Inside(player->Pos(),
		Region::halfsize))
	{
		player->Steering()->SetTarget(player->Pos());
		player->GetFSM()->ChangeState(&GetInstObj(Wait));
	}
	//if game is not on the player must return much closer to the center of his
	//home region
	else if(!player->Pitch()->GameOn() && player->AtTarget())
	{
		player->GetFSM()->ChangeState(&GetInstObj(Wait));
	}
}

void ReturnToHomeRegion::Exit(FieldPlayer* player)
{
	player->Steering()->ArriveOff();
}


EmptyMsg(bool,Wait,OnMessage,FieldPlayer);
EmptyFun(void,Wait,Exit,FieldPlayer);

void Wait::Enter(FieldPlayer* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 Wait x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 Wait x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters wait state" << "";
#endif

	//if the game is not on make sure the target is the center of the player's
	//home region. This is ensure all the players are in the correct positions
	//ready for kick off
	if (!player->Pitch()->GameOn())
	{
		player->Steering()->SetTarget(player->HomeRegion()->Center());
	}
}

void Wait::Execute(FieldPlayer* player)
{    
	if( !player->Team()->IsChaseBall() ) return;

	if( player->GetID() == 11 )
	{
		char p ='1';
		p = '0';
	}
	/// 如果队员被挤出位置，那么要回来原来位置
	if (!player->AtTarget())
	{
		player->Steering()->ArriveOn();

		return;
	}
	else
	{
		player->Steering()->ArriveOff();

		player->SetVelocity(Vector2D(0,0));

		///该队员盯球
		player->TrackBall();
	}

	///
	/// 如果该队员的球正在控制着球，改队员不是进攻队员，
	/// 并球该队员比进攻队员更靠近前场
	/// 那么他应该请求传球
	/// 
	if ( player->Team()->InControl()    &&
		(!player->isControllingPlayer()) &&
		player->isAheadOfAttacker() )
	{
		player->Team()->RequestPass(player);

		return;
	}

	if (player->Pitch()->GameOn())
	{
		/// 如果该队员是球队中离球最近的，球队也没有分配接球队员，同时守门员没有拿着球，那么去追球
		if (player->isClosestTeamMemberToBall() &&
			player->Team()->Receiver() == NULL  &&
			!player->Pitch()->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

			return;
		}

		/// 如果太远需要归位
		if( !player->IsSelfRegin() && (player->Team()->InControl() ||  player->FollowReturn() ) )
		{
			player->Steering()->SetTarget(player->HomeRegion()->Center());
			player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion));
		}


		/// 否则跟随球		  && player->Role() == FieldPlayer::defender
		if ( !player->Team()->InControl() && player->FollowTarget()  )
		{
			 player->Steering()->SetTarget(player->Ball()->Pos());
			 player->GetFSM()->ChangeState(&GetInstObj(FollowBall));
		}

	} 
}


EmptyMsg(bool,FollowBall,OnMessage,FieldPlayer);

void FollowBall::Enter(FieldPlayer* player)
{
	player->Steering()->SeekOn();
}

void FollowBall::Execute(FieldPlayer* player)
{
	if( !player->Team()->IsChaseBall() ) return;

	if (player->Pitch()->GameOn())
	{
		/// 如果太远需要归位
		if( player->Team()->InControl() ||  player->FollowReturn() )
		{
			player->Steering()->SetTarget(player->HomeRegion()->Center());
			player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion));
		}
		
		/// 不是最近队员，是防御者，不是控球队员，需要进行跟踪球进行防御	 &&
		///	 player->Role() == FieldPlayer::defender 
		if (!player->isClosestTeamMemberToBall() &&
			!player->Team()->InControl()  )
		{
			player->Steering()->SetTarget(player->Ball()->Pos());
			return;
		}

		/// 如果是最近的，且现在是边界球，应该去发球
		if (player->isClosestTeamMemberToBall() &&
			player->Team()->IsThrowIn() )
		{
			player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

			return;
		}

		/// 如果该队员是球队中离球最近的，球队也没有分配接球队员，同时守门员没有拿着球，那么去追球
		if (player->isClosestTeamMemberToBall() &&
			player->Team()->Receiver() == NULL  &&
			!player->Pitch()->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

			return;
		}
	}
}

void FollowBall::Exit(FieldPlayer *player)
{
	player->Steering()->SeekOff();
}


EmptyMsg(bool,KickBall,OnMessage,FieldPlayer);

void KickBall::Enter(FieldPlayer* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 KickBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 KickBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}

	/// 使球队直到该队员正在控制球
	player->Team()->SetControllingPlayer(player);

	/// 该队员每秒只能惊喜有限次数的踢球
	if (!player->isReadyForNextKick()) 
	{
		player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));
	}


#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters kick state" << "";
#endif
}

EmptyFun(void,KickBall,Exit,FieldPlayer);

void KickBall::Execute(FieldPlayer* player)
{ 
	if( !player->Team()->IsChaseBall() ) 
	{
		player->GetFSM()->ChangeState(&GetInstObj(Wait));
		return;
	}

	/// 计算指向球的向量与球员自己的朝向向量的点积
	Vector2D ToBall = player->Ball()->Pos() - player->Pos();
	double   dot    = player->Heading().Dot(Vec2DNormalize(ToBall)); 

	/// 如果守门员控制了球，或者球还在该队员的后面，
	/// 或者已经分配了接球队员，就不能踢球，所以只是继续追球
	if (player->Team()->Receiver() != NULL   ||
		player->Pitch()->GoalKeeperHasBall() ||
		(dot < 0) ) 
	{
#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Goaly has ball / ball behind player" << "";
#endif

		player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

		return;
	}

	/// 计算指向球的向量与球员自己的朝向向量的点积
	Vector2D    BallTarget;

	/// 计算指向球的向量与球员自己的朝向向量的点积
	double power = GetInstObj(CGameSetup).MaxShootingForce * dot;

	/// 如果确认该队员可以在这个位置射门，或者无论如何他都改该踢一下球，那该队员则试图射门
	if (player->Team()->CanShoot(player->Ball()->Pos(),
		power,
		BallTarget)                   || 
		(RandFloat() < GetInstObj(CGameSetup).ChancePlayerAttemptsPotShot))
	{
#ifdef PLAYER_STATE_INFO_ON
		// debug_con << "Player " << player->GetID() << " attempts a shot at " << BallTarget << "";
#endif

		/// 给射门增加一些干扰，我们不想让队员踢得太准，
		/// 通过改变PlayerKickingAccuracy值可以调整干扰数值
		BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

		///这是踢球的方向
		Vector2D KickDirection = BallTarget - player->Ball()->Pos();

		player->Ball()->Kick(KickDirection, power,player);

		//change state   
		player->GetFSM()->ChangeState(&GetInstObj(Wait));

		player->FindSupport();

		player->Team()->SetThrowIn(false);
		return;
	}

	/// 找到接球队员，那么receiver将指向他
	PlayerBase* receiver = NULL;

	power = GetInstObj(CGameSetup).MaxPassingForce * dot;

	/// 测试是否有任何潜在的候选人可以接球
	//test if there are any potential candidates available to receive a pass
	if (player->isThreatened()  &&
		player->Team()->FindPass(player,
		receiver,
		BallTarget,
		power,
		GetInstObj(CGameSetup).MinPassDist))
	{     
		/// 给踢球增加一些干扰
		BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

		Vector2D KickDirection = BallTarget - player->Ball()->Pos();

		player->Ball()->Kick(KickDirection, power,player);

		player->Team()->SetThrowIn(false);

#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Player " << player->GetID() << " passes the ball with force " << power << "  to player " 
		//             << receiver->ID() << "  Target is " << BallTarget << "";
#endif


		/// 让接球队员知道要传球
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			player->GetID(),
			receiver->GetID(),
			Msg_ReceiveBall,
			&BallTarget);                            


		/// 该队员应该等在他的当前位置，除非另有指示
		player->GetFSM()->ChangeState(&GetInstObj(Wait));

		player->FindSupport();

		return;
	}

	/// 不能射门和传球，只能带球到前场
	else
	{   
		player->FindSupport();

		player->GetFSM()->ChangeState(&GetInstObj(Dribble));
	}   
}

EmptyFun(void,Dribble,Exit,FieldPlayer);
EmptyMsg(bool,Dribble,OnMessage,FieldPlayer);

void Dribble::Enter(FieldPlayer* player)
{

	if( !player->Team()->IsChaseBall() ) return;

	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 Dribble x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 Dribble x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}

	/// 玩家正在控球
	player->Team()->SetControllingPlayer(player);

#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters dribble state" << "";
#endif
}

void Dribble::Execute(FieldPlayer* player)
{

	if( !player->Team()->IsChaseBall() ) return;

	double dot = player->Team()->HomeGoalFacing().Dot(player->Heading());


	/// 如果球在队员和自己方球们之间，他们需要通过多次轻踢和小转弯
	if (dot < 0)
	{
		
		/// 队员的朝向稍微转移下（PI/4），然后在那个方向踢球
		Vector2D direction = player->Heading();

		/// 计算队员的朝向和球门的朝向之间角度的正负号(+/-)
		/// 使得队员可以转到正确方向
		double angle = QuarterPi * -1 *
			player->Team()->HomeGoalFacing().Sign(player->Heading());

		Vec2DRotateAroundOrigin(direction, angle);

		/// 当队员正在实体控制球，且同时转弯时，这个值起到很好的作用
		const double KickingForce = 0.8;

		player->Ball()->Kick(direction, KickingForce,player);
		player->Team()->SetThrowIn(false);
	}

	/// 踢球
	//kick the ball down the field
	else
	{
		player->Ball()->Kick(player->Team()->HomeGoalFacing(),
			GetInstObj(CGameSetup).MaxDribbleForce,player); 
		player->Team()->SetThrowIn(false);
	}

	/// 改队员已经踢球了，所以他必须改变状态去追球
	//the player has kicked the ball so he must now change state to follow it
	player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

	return;  
}


EmptyMsg(bool,ReceiveBall,OnMessage,FieldPlayer);

void ReceiveBall::Enter(FieldPlayer* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 ReceiveBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 ReceiveBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}

	/// 让球员知道，这个队员正在接球
	player->Team()->SetReceiver(player);

	/// 该队员现在也是控球队员
	//this player is also now the controlling player
	player->Team()->SetControllingPlayer(player);

	/// 有2类控球行为，1.用arrive指导接球队员到达传球队员发送的消息中制定的位置。
	/// 2.用Pursuit行为来追逐球
	/// 这个语句依据ChanceOfUsingArriveTypeReceiveBehavior的可能性选择其一，判断是否有对方队员靠近接球队员
	/// 是否接球队员在对方的热区（所有队员中离对方球门第三近的）
	const double PassThreatRadius = 70.0;

	if (( player->InHotRegion() ||
		RandFloat() < GetInstObj(CGameSetup).ChanceOfUsingArriveTypeReceiveBehavior) &&
		!player->Team()->isOpponentWithinRadius(player->Pos(), PassThreatRadius))
	{
		player->Steering()->ArriveOn();

#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Player " << player->GetID() << " enters receive state (Using Arrive)" << "";
#endif
	}
	else
	{
		player->Steering()->PursuitOn();

#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Player " << player->GetID() << " enters receive state (Using Pursuit)" << "";
#endif
	}
}

void ReceiveBall::Execute(FieldPlayer* player)
{
	if( !player->Team()->IsChaseBall() ) return;

	/// 如果他离球足够近或者他的球队市区球的控制权，那么他应该状态去追求
	if (player->BallWithinReceivingRange() || !player->Team()->InControl())
	{
		player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

		return;
	}  

	/// 如果Pursuit操控行为被用来追球，那么该队员的目标必须随着球的位置不断的更新
	if (player->Steering()->PursuitIsOn())
	{
		player->Steering()->SetTarget(player->Ball()->Pos());
	}

	/// 如果改队员达到了操控目标的位置，那么它应该等在哪儿，并转向面对着球
	if (player->AtTarget())
	{
		player->Steering()->ArriveOff();
		player->Steering()->PursuitOff();
		player->TrackBall();    
		player->SetVelocity(Vector2D(0,0));
	} 
}

void ReceiveBall::Exit(FieldPlayer* player)
{
	player->Steering()->ArriveOff();
	player->Steering()->PursuitOff();

	player->Team()->SetReceiver(NULL);
}








