#include "Stdafx.h"
#include "StateMachine.h"
#include "../Entity/PlayerBase.h"
#include "../Entity/FieldPlayer.h"
#include "FieldPlayerStates.h"		  
#include "../Goal.h"
#include "../SoccerTeam.h"
#include "../SoccerPitch.h"
#include "../Messageing/MessageDispatcher.h"
#include "../Messageing/SoccerMessages.h"
#include "../SteeringBehaviors.h"
#include "../Telegram.h"
#include "../../Config.h"	


#define PLAYER_STATE_INFO_ON


//************************************************************************ Global state

GlobalPlayerState* GlobalPlayerState::Instance()
{
	static GlobalPlayerState instance;

	return &instance;
}


void GlobalPlayerState::Execute(FieldPlayer* player)                                     
{
	if( !player->Team()->IsChaseBall() ) return;

	/// 如果球员占有并接近球，那么降低他的最大速度
	//if a player is in possession and close to the ball reduce his max speed
	if((player->BallWithinReceivingRange()) && (player->isControllingPlayer()))
	{
		player->SetMaxSpeed(GetInstObj(CGameSetup).PlayerMaxSpeedWithBall);
	}

	else
	{
		player->SetMaxSpeed(GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall);
	}

}


bool GlobalPlayerState::OnMessage(FieldPlayer* player, const Telegram& telegram)
{
	switch(telegram.Msg)
	{
	case Msg_ReceiveBall:
		{
			//set the target
			player->Steering()->SetTarget(*(static_cast<Vector2D*>(telegram.ExtraInfo)));

			//change state 
			player->GetFSM()->ChangeState(ReceiveBall::Instance());

			return true;
		}

		break;

	case Msg_SupportAttacker:
		{
			//如果已经在接应，立即返回
			if (player->GetFSM()->isInState(*SupportAttacker::Instance()))
			{
				return true;
			}

			/// 设置目标最佳接应位置
			//set the target to be the best supporting position
			player->Steering()->SetTarget(player->Team()->GetSupportSpot());

			//change the state
			player->GetFSM()->ChangeState(SupportAttacker::Instance());

			return true;
		}

		break;

	case Msg_Wait:
		{
			//change the state
			player->GetFSM()->ChangeState(Wait::Instance());

			return true;
		}

		break;

	case Msg_GoHome:
		{
			player->SetDefaultHomeRegion();

			player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance());

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
			//if the ball is not within kicking range or their is already a 
			//receiving player, this player cannot pass the ball to the player
			//making the request.
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
			//let the receiver know a pass is coming 
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
				player->GetID(),
				receiver->GetID(),
				Msg_ReceiveBall,
				&receiver->Pos());



			//change state   
			player->GetFSM()->ChangeState(Wait::Instance());

			player->FindSupport();

			return true;
		}

		break;

	}//end switch

	return false;
}




//***************************************************************************** CHASEBALL

ChaseBall* ChaseBall::Instance()
{
	static ChaseBall instance;

	return &instance;
}


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
	//if the ball is within kicking range the player changes state to KickBall.
	if (player->BallWithinKickingRange())
	{
		player->GetFSM()->ChangeState(KickBall::Instance());

		return;
	}
	
	/// 如果改球队员离球最近，继续追球
	//if the player is the closest player to the ball then he should keep
	//chasing it
	if (player->isClosestTeamMemberToBall())
	{
		player->Steering()->SetTarget(player->Ball()->Pos());

		return;
	}

	/// 如果该队员不是离球最近，那么他应该回到自己的初始位置，等待下一次
	//if the player is not closest to the ball anymore, he should return back
	//to his home region and wait for another opportunity
	player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance());
}


void ChaseBall::Exit(FieldPlayer* player)
{
	player->Steering()->SeekOff();
}



//*****************************************************************************SUPPORT ATTACKING PLAYER

SupportAttacker* SupportAttacker::Instance()
{
	static SupportAttacker instance;

	return &instance;
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

void SupportAttacker::Execute(FieldPlayer* player)                                     
{
	if( !player->Team()->IsChaseBall() ) return;

	/// 如果自己球队丢失对球的控制权，球员回到起始区域
	//if his team loses control go back home
	if (!player->Team()->InControl())
	{
		player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance()); return;
	} 

	/// 如果最佳接应点改变了，那么改变操控目标
	//if the best supporting spot changes, change the steering target
	if (player->Team()->GetSupportSpot() != player->Steering()->Target())
	{    
		player->Steering()->SetTarget(player->Team()->GetSupportSpot());

		player->Steering()->ArriveOn();
	}

	/// 如果这么球员可以射门，切进攻队员可以把球传给他，那么把球传给该队员
	//if this player has a shot at the goal AND the attacker can pass
	//the ball to him the attacker should pass the ball to this player
	if( player->Team()->CanShoot(player->Pos(),
		GetInstObj(CGameSetup).MaxShootingForce))
	{
		player->Team()->RequestPass(player);
	}

	/// 如果这名队员在接应点，且他的球队仍控制球，他应该停在那儿，转向面对着球
	//if this player is located at the support spot and his team still have
	//possession, he should remain still and turn to face the ball
	if (player->AtTarget())
	{
		player->Steering()->ArriveOff();

		/// 队员跟踪球
		//the player should keep his eyes on the ball!
		player->TrackBall();

		player->SetVelocity(Vector2D(0,0));

		/// 如果没有收到其他队员的威胁，那么请求传球
		//if not threatened by another player request a pass
		if (!player->isThreatened())
		{
			player->Team()->RequestPass(player);
		}
	}
}


void SupportAttacker::Exit(FieldPlayer* player)
{
	//set supporting player to null so that the team knows it has to 
	//determine a new one.
	player->Team()->SetSupportingPlayer(NULL);

	player->Steering()->ArriveOff();
}




//************************************************************************ RETURN TO HOME REGION

ReturnToHomeRegion* ReturnToHomeRegion::Instance()
{
	static ReturnToHomeRegion instance;

	return &instance;
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

	if (!player->HomeRegion()->Inside(player->Steering()->Target(), Region::halfsize))
	{
		player->Steering()->SetTarget(player->HomeRegion()->Center());
	}

#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters ReturnToHome state" << "";
#endif
}

void ReturnToHomeRegion::Execute(FieldPlayer* player)
{
	if( !player->Team()->IsChaseBall() ) return;

	if (player->Pitch()->GameOn())
	{
		//if the ball is nearer this player than any other team member  &&
		//there is not an assigned receiver && the goalkeeper does not gave
		//the ball, go chase it
		if ( player->isClosestTeamMemberToBall() &&
			(player->Team()->Receiver() == NULL) &&
			!player->Pitch()->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(ChaseBall::Instance());

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
		player->GetFSM()->ChangeState(Wait::Instance());
	}
	//if game is not on the player must return much closer to the center of his
	//home region
	else if(!player->Pitch()->GameOn() && player->AtTarget())
	{
		player->GetFSM()->ChangeState(Wait::Instance());
	}
}

void ReturnToHomeRegion::Exit(FieldPlayer* player)
{
	player->Steering()->ArriveOff();
}




//***************************************************************************** WAIT

Wait* Wait::Instance()
{
	static Wait instance;

	return &instance;
}


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

	/// 如果队员被挤出位置，那么要回来原来位置
	//if the player has been jostled out of position, get back in position  
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
		//the player should keep his eyes on the ball!
		player->TrackBall();
	}

	/// 如果该队员的球正在控制着球，改队员不是进攻队员，冰球该队员比进攻队员更靠近前场
	/// 那么他应该请求传球
	//if this player's team is controlling AND this player is not the attacker
	//AND is further up the field than the attacker he should request a pass.
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
		//if the ball is nearer this player than any other team member  AND
		//there is not an assigned receiver AND neither goalkeeper has
		//the ball, go chase it
		if (player->isClosestTeamMemberToBall() &&
			player->Team()->Receiver() == NULL  &&
			!player->Pitch()->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(ChaseBall::Instance());

			return;
		}
	} 
}

void Wait::Exit(FieldPlayer* player){}




//************************************************************************ KICK BALL

KickBall* KickBall::Instance()
{
	static KickBall instance;

	return &instance;
}


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
	//let the team know this player is controlling
	player->Team()->SetControllingPlayer(player);

	/// 该队员每秒只能惊喜有限次数的踢球
	//the player can only make so many kick attempts per second.
	if (!player->isReadyForNextKick()) 
	{
		player->GetFSM()->ChangeState(ChaseBall::Instance());
	}


#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters kick state" << "";
#endif
}

void KickBall::Execute(FieldPlayer* player)
{ 
	if( !player->Team()->IsChaseBall() ) 
	{
		player->GetFSM()->ChangeState(Wait::Instance());
		return;
	}

	/// 计算指向球的向量与球员自己的朝向向量的点积
	//calculate the dot product of the vector pointing to the ball
	//and the player's heading
	Vector2D ToBall = player->Ball()->Pos() - player->Pos();
	double   dot    = player->Heading().Dot(Vec2DNormalize(ToBall)); 

	/// 如果守门员控制了球，或者球还在该队员的后面，
	/// 或者已经分配了接球队员，就不能踢球，所以只是继续追球
	//cannot kick the ball if the goalkeeper is in possession or if it is 
	//behind the player or if there is already an assigned receiver. So just
	//continue chasing the ball
	if (player->Team()->Receiver() != NULL   ||
		player->Pitch()->GoalKeeperHasBall() ||
		(dot < 0) ) 
	{
#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Goaly has ball / ball behind player" << "";
#endif

		player->GetFSM()->ChangeState(ChaseBall::Instance());

		return;
	}

	/* Attempt a shot at the goal */

	/// 计算指向球的向量与球员自己的朝向向量的点积
	//if a shot is possible, this vector will hold the position along the 
	//opponent's goal line the player should aim for.
	Vector2D    BallTarget;

	/// 计算指向球的向量与球员自己的朝向向量的点积
	//the dot product is used to adjust the shooting force. The more
	//directly the ball is ahead, the more forceful the kick
	double power = GetInstObj(CGameSetup).MaxShootingForce * dot;

	/// 如果确认该队员可以在这个位置射门，或者无论如何他都改该踢一下球，那该队员则试图射门
	//if it is determined that the player could score a goal from this position
	//OR if he should just kick the ball anyway, the player will attempt
	//to make the shot
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
		//add some noise to the kick. We don't want players who are 
		//too accurate! The amount of noise can be adjusted by altering
		//GetInstObj(CGameSetup).PlayerKickingAccuracy
		BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

		///这是踢球的方向
		//this is the direction the ball will be kicked in
		Vector2D KickDirection = BallTarget - player->Ball()->Pos();

		player->Ball()->Kick(KickDirection, power,player);

		//change state   
		player->GetFSM()->ChangeState(Wait::Instance());

		player->FindSupport();

		player->Team()->SetThrowIn(false);
		return;
	}


	/* Attempt a pass to a player */

	/// 找到接球队员，那么receiver将指向他
	//if a receiver is found this will point to it
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
		//add some noise to the kick
		BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

		Vector2D KickDirection = BallTarget - player->Ball()->Pos();

		player->Ball()->Kick(KickDirection, power,player);

		player->Team()->SetThrowIn(false);

#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Player " << player->GetID() << " passes the ball with force " << power << "  to player " 
		//             << receiver->ID() << "  Target is " << BallTarget << "";
#endif


		/// 让接球队员知道要传球
		//let the receiver know a pass is coming 
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			player->GetID(),
			receiver->GetID(),
			Msg_ReceiveBall,
			&BallTarget);                            


		/// 该队员应该等在他的当前位置，除非另有指示
		//the player should wait at his current position unless instruced
		//otherwise  
		player->GetFSM()->ChangeState(Wait::Instance());

		player->FindSupport();

		return;
	}

	/// 不能射门和传球，只能带球到前场
	//cannot shoot or pass, so dribble the ball upfield
	else
	{   
		player->FindSupport();

		player->GetFSM()->ChangeState(Dribble::Instance());
	}   
}


//*************************************************************************** DRIBBLE

Dribble* Dribble::Instance()
{
	static Dribble instance;

	return &instance;
}


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
	//let the team know this player is controlling
	player->Team()->SetControllingPlayer(player);

#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters dribble state" << "";
#endif
}

void Dribble::Execute(FieldPlayer* player)
{

	if( !player->Team()->IsChaseBall() ) return;

	double dot = player->Team()->HomeGoal()->Facing().Dot(player->Heading());


	/// 如果球在队员和自己方球们之间，他们需要通过多次轻踢和小转弯
	//if the ball is between the player and the home goal, it needs to swivel
	// the ball around by doing multiple small kicks and turns until the player 
	//is facing in the correct direction
	if (dot < 0)
	{
		
		/// 队员的朝向稍微转移下（PI/4），然后在那个方向踢球
		//the player's heading is going to be rotated by a small amount (Pi/4) 
		//and then the ball will be kicked in that direction
		Vector2D direction = player->Heading();

		/// 计算队员的朝向和球门的朝向之间角度的正负号(+/-)
		/// 使得队员可以转到正确方向
		//calculate the sign (+/-) of the angle between the player heading and the 
		//facing direction of the goal so that the player rotates around in the 
		//correct direction
		double angle = QuarterPi * -1 *
			player->Team()->HomeGoal()->Facing().Sign(player->Heading());

		Vec2DRotateAroundOrigin(direction, angle);

		/// 当队员正在实体控制球，且同时转弯时，这个值起到很好的作用
		//this value works well whjen the player is attempting to control the
		//ball and turn at the same time
		const double KickingForce = 0.8;

		player->Ball()->Kick(direction, KickingForce,player);
		player->Team()->SetThrowIn(false);
	}

	/// 踢球
	//kick the ball down the field
	else
	{
		player->Ball()->Kick(player->Team()->HomeGoal()->Facing(),
			GetInstObj(CGameSetup).MaxDribbleForce,player); 
		player->Team()->SetThrowIn(false);
	}

	/// 改队员已经踢球了，所以他必须改变状态去追球
	//the player has kicked the ball so he must now change state to follow it
	player->GetFSM()->ChangeState(ChaseBall::Instance());

	return;  
}



//************************************************************************     RECEIVEBALL

ReceiveBall* ReceiveBall::Instance()
{
	static ReceiveBall instance;

	return &instance;
}


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
	//let the team know this player is receiving the ball
	player->Team()->SetReceiver(player);

	/// 该队员现在也是控球队员
	//this player is also now the controlling player
	player->Team()->SetControllingPlayer(player);

	//there are two types of receive behavior. One uses arrive to direct
	//the receiver to the position sent by the passer in its telegram. The
	//other uses the pursuit behavior to pursue the ball. 
	//This statement selects between them dependent on the probability
	//ChanceOfUsingArriveTypeReceiveBehavior, whether or not an opposing
	//player is close to the receiving player, and whether or not the receiving
	//player is in the opponents 'hot region' (the third of the pitch closest
	//to the opponent's goal
	/// 有2类控球行为，1.用arrive指导接球队员到达传球队员发送的消息中制定的位置，2.
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
	//if the ball comes close enough to the player or if his team lose control
	//he should change state to chase the ball
	if (player->BallWithinReceivingRange() || !player->Team()->InControl())
	{
		player->GetFSM()->ChangeState(ChaseBall::Instance());

		return;
	}  

	/// 如果Pursuit操控行为被用来追球，那么该队员的目标必须随着球的位置不断的更新
	if (player->Steering()->PursuitIsOn())
	{
		player->Steering()->SetTarget(player->Ball()->Pos());
	}

	/// 如果改队员达到了操控目标的位置，那么它应该等在哪儿，并转向面对着球
	//if the player has 'arrived' at the steering target he should wait and
	//turn to face the ball
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








