
State_GlobalPlayer = { }

State_GlobalPlayer["Enter"] = function(player)

end


State_GlobalPlayer["Execute"] = function(player)

	if not player:IsChaseBall() then

		return;

	end

	--
	if player:BallWithinReceivingRange() and player:isControllingPlayer() then

		player:SetMaxSpeed(1.2);

	else
		player:SetMaxSpeed(1.6);
	end

end

State_GlobalPlayer["OnMessage"] = function(player,Msg)

	if Msg:GetMsg() == 0 then

		player:Steering():SetTarget( Msg:GetVec() )
		player:GetFSM():ChangeState( State_ReceiveBall );

		return 1;

	elseif Msg:GetMsg() == 1 then


		local receiver = GetExtraInfoField(Msg) ;

		if player:Team():IsReceiver() or not player:BallWithinKickingRange() then

			return 1;

		end

		local subvec = Vec2DSub(receiver:Pos(),player():Ball():Pos());


		player:Ball():Kick( subvec , 3 , player ) ;


		local Dispatcher = MsgDispatcher();

		local addrvec =  VecAddr(receiver:Pos());

		Dispatcher:DispatchMsg( 0,player:ID(),receiver:ID(), addrvec );

		player:GetFSM():ChangeState( State_Wait );

		player:FindSupport();

		return 1;


	elseif Msg:GetMsg() == 2 then

		if player:GetFSM():IsInState ( State_SupportAttacker ) then
			return 1;
		end

		player:Steering():SetTarget( player:Team():GetSupportSpot() );
		player:GetFSM():ChangeState( State_SupportAttacker );

		return 1

	elseif Msg:GetMsg() == 3 then

	    player:SetDefaultHomeRegion();
		player:GetFSM():ChangeState( State_ReturnGoHome );

	elseif Msg:GetMsg() == 4 then

		player:GetFSM():ChangeState( State_Wait );
		return 1;

	end
end


State_GlobalPlayer["Exit"] = function(player)

end



State_ChaseBall = {}

State_ChaseBall["Enter"] = function(player)

	player:Steering():SeekOn();

end

State_ChaseBall["Execute"] = function(player)

	if player:BallWithinKickingRange() then

		player:GetFSM():ChangeState(State_KickBall)
		return;

	end

	if  player:isClosestTeamMemberToBall() then
		player:Steering():SetTarget( player:Ball():Pos() );
		return;
	end

	player:GetFSM():ChangeState(State_ReturnGoHome);

end

State_ChaseBall["OnMessage"] = function(player,Msg)
	return 0;
end


State_ChaseBall["Exit"] = function(player)

	player:Steering():SeekOff();

end


State_SupportAttacker = { }

State_SupportAttacker["OnMessage"] = function(player,Msg)

	return 0;

end


State_SupportAttacker["Enter"] = function(player)

	player:Steering():ArriveOn();
	player:Steering():SetTarget( player:Team():GetSupportSpot() )

end


State_SupportAttacker["Execute"] = function(player)

	if player:IsChaseBall() then
		return;
	end

	if  player:Team():IsControl() then

		player:GetFSM():ChangeState(State_ReturnGoHome);

	end


	if  type(player:Team():GetSupportSpot() ) ~= type( player:Steering():Target() ) then

		player:Steering():SetTarget(player:Team():GetSupportSpot() );
		player:Steering():ArriveOn();

	end

	if  player:Team():CanShoot(player:Pos(),6) then

		player:Team():RequestPass(player)

	end

	if player:AtTarget() then

		player:Steering():ArriveOff();

		player:TrackBall();

		player:SetVelocity();

		if  not player:isThreatened() then

			player:Team():RequestPass(player)

		end

	end

end

State_SupportAttacker["Exit"] = function(player)

	player:Team():SetSupportingPlayer();

	player:Steering():ArriveOff();

end



State_ReturnGoHome = {}


State_ReturnGoHome["OnMessage"] = function(player,Msg)

	return 0;

end


State_ReturnGoHome["Enter"] = function(player)

	player:Steering():ArriveOn();

	if not player:InSideHomeRegion( player:Steering():Target(), 0 ) then

		player:Steering():SetTarget( player:GetHomeCenter() )

	end

end


State_ReturnGoHome["Execute"] = function(player)

	if  not player:IsChaseBall() then

		return;

	end

	if  player:Pitch():GameOn()	then

		if player:isClosestTeamMemberToBall() and player:Team():IsReceiver() and  not player:Pitch():GoalKeeperHasBall() then

			player:GetFSM():ChangeState(State_ChaseBall);

		end

	end

	if player:Pitch():GameOn() and  player:InSideHomeRegion( player:Pos(), 1 ) then

		player:Steering():SetTarget( player:Pos() );
		player:GetFSM():ChangeState(State_Wait);

	elseif not player:Pitch():GameOn() and player:AtTarget() then

		player:GetFSM():ChangeState(State_Wait);

	end

end


State_ReturnGoHome["Exit"] = function(player)

	player:Steering():ArriveOff();

end


State_Wait = { }


State_Wait["OnMessage"] = function(player,Msg)

	return 0;

end

State_Wait["Enter"] = function(player)

	if  not player:Pitch():GameOn()	then

		player:Steering():SetTarget( player:GetHomeCenter() )

	end

end



State_Wait["Execute"] = function(player)

	if not player:IsChaseBall() then
		return;
	end

	if not player:AtTarget() then

		player:Steering():ArriveOn();

		return;

	else
		 player:Steering():ArriveOff();
		 local  vec = Vector2D();
		 player:SetVelocity(vec);
		 player:TrackBall();

	end


	if not player:isControllingPlayer()  and player:Team():IsControl() and  player:isAheadOfAttacker()  then

				player:Team():RequestPass(player);

				return;
	end


	if  player:Pitch():GameOn() then

		if player:isClosestTeamMemberToBall() and player:Team():IsReceiver() and  not player:Pitch():GoalKeeperHasBall() then

			player:GetFSM():ChangeState( State_ChaseBall )

			return ;

		end

		if not player:IsSelfRegin() then

			if player:Team():IsControl() or player:FollowReturn() then

				player:Steering():SetTarget( player:GetHomeCenter() );
				player:GetFSM():ChangeState( State_ReturnGoHome );

			end

		end

		if not player:Team():IsControl() and player:FollowTarget() then

			player:Steering():SetTarget( player:Ball():Pos() )
			player:GetFSM():ChangeState( State_FollowBall )

		end

	end


end

State_Wait["Exit"] = function(player)


end



State_FollowBall = { }


State_FollowBall["OnMessage"] = function(player,Msg)

	return 0;

end

State_FollowBall["Enter"] = function(player)

	player:Steering():SeekOn();

end

State_FollowBall["Execute"] = function(player)

	if not player:IsChaseBall() then

		return;

	end

	if player:Pitch():GameOn() then

		if  player:Team():IsControl() or player:FollowReturn() then

			player:Steering():SetTarget( player:GetHomeCenter() )
			player:GetFSM():ChangeState( State_ReturnGoHome )

		end

		if not player:isClosestTeamMemberToBall() and  not player:Team():IsControl() then

			player:Steering():SetTarget( player:Ball():Pos() );

		end

		if player:isClosestTeamMemberToBall() and player:Team():IsThrowIn() then

			player:GetFSM():ChangeState( State_ChaseBall );

		end

		if player:isClosestTeamMemberToBall() and player:Team():IsReceiver() and not player:Pitch():GoalKeeperHasBall() then

			player:GetFSM():ChangeState( State_ChaseBall );
		end

	end

end


State_FollowBall["Exit"] = function(player)

	player:Steering():SeekOff();

end



State_KickBall = { }


State_KickBall["OnMessage"] = function(player,Msg)

	return 0;

end

State_KickBall["Enter"] = function(player)

	player:Team():SetControllingPlayer( player );

	if not player:isReadyForNextKick() then

		player:GetFSM():ChangeState( State_ChaseBall )

	end
end


State_KickBall["Execute"] = function(player)

	if not player:IsChaseBall() then

		player:GetFsm():ChangeState( State_Wait )

	end

	local ToBall = Vec2DSub( player:Ball():Pos(), player:Pos());

	local Vec2D  = Vec2DNormalize(ToBall);

	local dot	  = player:Heading():Dot(Vec2D);

	if player:Team():IsReceiver() or player:Pitch():GoalKeeperHasBall() or dot < 0 then

		player:GetFsm():ChangeState( State_ChaseBall );

	end

	local  BallTarget = Vector2D();

	local  power = 6 * dot;

	if player:Team():CanShoot( player:Ball():Pos() , power , BallTarget ) or  RandFloat() <  0.005 then

		BallTarget = AddNoiseToKick( player:Ball():Pos() , BallTarget );

		local  kickDirection = Vec2DSub(BallTarget,player:Ball():Pos());

		player:Ball():Kcik( kickDirection , power , player );

		player:GetFSM():ChangeState( State_Wait )

		player:FindSupport();

		player:Team():SetThrowIn( false );

	end


	local receiver;

	power = 3 * dot;


	if player:isThreatened() and player:Team():FindPass( player, receiver, BallTarget,power,100 ) then

		BallTarget = AddNoiseToKick( player:Ball():Pos() , BallTarget );

		local  kickDirection = Vec2DSub( BallTarget , player:Ball():Pos() );

		player:Ball():Kcik( kickDirection , power , player );


		player:Team():SetThrowIn( false );

		local  Dispatcher = MsgDispatcher();

		local  addrvec    = VecAddr( BallTarget );

		Dispatcher:DispatchMsg( 0 , player:ID() , receiver:ID() , 0 , addrvec );

		-- 传球消息公告

		player:GetFSM():ChangeState( State_Wait )

		player:FindSupport();

	else

		player:FindSupport();

		player:GetFSM():ChangeState( State_Dribble )

	end
	--local


end


State_KickBall["Exit"] = function(player)

	if not player:isReadyForNextKick() then

		player:GetFSM():ChangeState( State_ChaseBall )

	end

end

State_Dribble = {}

State_Dribble["OnMessage"] = function(player,Msg)

	return 0;

end

State_Dribble["Enter"] = function(player)

	if not player:IsChaseBall() then
		return ;
	end

	player:Team():SetControllingPlayer( player )

end


State_Dribble["Execute"] = function(player)

	if not player:IsChaseBall() then
		return ;
	end


	local  dot = player:Team():HomeGoalFacing():Dot( player:Heading() );

	if  dot < 0 then

		local  direction = player:Heading();

		local  angle = 3.1415926/4 * -1 * player:Team():HomeGoalFacing():Sign( player:Heading() );

		Vec2DRotateAroundOrigin( direction , angle );

		local  KickingForce = 0.8;

		player:Ball():Kick( direction , KickingForce , player );

		player:Team():SetThrowIn(false);

	else

		player:Ball():Kick( player:Team():HomeGoalFacing() , 1.5 , player );

		player:Team():SetThrowIn(false);

	end


	player:GetFSM():ChangeState( State_ChaseBall )


end


State_Dribble["Exit"] = function(player)

end


State_ReceiveBall  = { }

State_ReceiveBall["OnMessage"] = function(player,Msg)

	return 0;

end


State_ReceiveBall["Enter"] = function(player)

	player:Team():SetReceiver(player);

	player:Team():SetControllingPlayer(player);

	local  PassThreatRadius = 70.0;

	if  player:InHotRegion() then

		player:Steering():ArriveOn();

	elseif RandFloat() < 0.5 and not player:Team():isOpponentWithinRadius(player:Pos(),PassThreatRadius) then

			 player:Steering():ArriveOn();
	else

		player:Steering():PursuitOn();

	end

end


State_ReceiveBall["Execute"] = function(player)

	if not player:IsChaseBall() then

		return;

	end

	if  player:BallWithinReceivingRange() or not player:Team():IsControl() then

		player:GetFSM():ChangeState(State_ChaseBall);

	end

	if player:Steering():PursuitIsOn() then

		player:Steering():SetTarget( player:Ball():Pos() );

	end

	if player:AtTarget() then

		player:Steering():ArriveOff();
		player:Steering():PursuitOff();

		player:TrackBall();
		player:SetVelocity();

	end

end


State_ReceiveBall["Exit"] = function(player)

	player:Steering():ArriveOff();
	player:Steering():PursuitOff();

	player:Team():SetReceiver();

end
