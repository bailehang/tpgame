
State_GlobalPlayer = { }

State_GlobalPlayer["Enter"] = function(player)

end


State_GlobalPlayer["Execute"] = function(player)

	PrintLuaMsg( player:ID() .. " State_GlobalPlayer Execute " );

	if not player:IsChaseBall() then

		return;

	end
	if player:BallWithinReceivingRange() and player:isControllingPlayer() then

		player:SetMaxSpeed(1.2);

	else
		player:SetMaxSpeed(1.6);

	end

end

State_GlobalPlayer["OnMessage"] = function(player,Msg)

	if Msg:GetMsg() == 0 then

		PrintLuaMsg( player:ID() .. " Msg:GetMsg() == 0 " );

		player:SetSteeringTarget( Msg:GetVec() );

		PrintLuaMsg( player:ID() .. " ChangeState GlobalPlayer change to State_ReceiveBall 26" );
		player:GetFSM():ChangeState( State_ReceiveBall );

		player:SetScriptValue( 1 );
		return;

	elseif Msg:GetMsg() == 1 then

		PrintLuaMsg( player:ID() .. " Msg:GetMsg() == 1 " );

		local RevPos = Msg:GetVec();

		local RevId  = GetGlobalEntityID( Msg:RevNum() );

		PrintLuaMsg( player:ID() .. " RevId = " .. RevId );

		if player:Team():IsReceiver() or not player:BallWithinKickingRange() then

			player:SetScriptValue( 1 );

			return;
		end

		PrintLuaMsg( player:ID() .. " RevId = " .. RevId );

		local subvec = Vec2DSub(RevPos,player():Ball():Pos());

		player:Ball():Kick( subvec , 3 , player ) ;

		--local Dispatcher = MsgDispatcher();

		--local addrvec =  VecAddr(RevPos);
		
		PrintLuaMsg( player:ID() .. " RevId = " .. RevId );

		DispatchVctMessage( player:ID(),RevId, 0 , RevPos );

		PrintLuaMsg( player:ID() .. " ChangeState GlobalPlayer change to State_Wait 25zxzxzx" );

		player:GetFSM():ChangeState( State_Wait );

		player:FindSupport();

		player:SetScriptValue( 1 );
		return;


	elseif Msg:GetMsg() == 2 then

		PrintLuaMsg( player:ID() .. " Msg:GetMsg() == 2 " );

		player:SetSteeringTarget( player:Team():GetSupportSpot() );

		PrintLuaMsg( player:ID() .. " ChangeState GlobalPlayer change to State_SupportAttacker 24" );

		player:GetFSM():ChangeState( State_SupportAttacker );

		player:SetScriptValue( 1 );
		return;

	elseif Msg:GetMsg() == 3 then

		PrintLuaMsg( player:ID() .. " Msg:GetMsg() == 3 " );

		player:SetDefaultHomeRegion();

		PrintLuaMsg( player:ID() .. " ChangeState GlobalPlayer change to State_ReturnGoHome 23" );

		player:GetFSM():ChangeState( State_ReturnGoHome );

		player:SetScriptValue( 1 );
		return;

	elseif Msg:GetMsg() == 4 then

		PrintLuaMsg( player:ID() .. " Msg:GetMsg() == 4 " );

		PrintLuaMsg( player:ID() .. " ChangeState GlobalPlayer change to State_Wait 22" );
		player:GetFSM():ChangeState( State_Wait );

		player:SetScriptValue( 1 );
		return;

	end
end


State_GlobalPlayer["Exit"] = function(player)

end



State_ChaseBall = {}

State_ChaseBall["Enter"] = function(player)

	PrintLuaMsg( player:ID() .. " Enter State_ChaseBall " );

	player:Steering():SeekOn();

end

State_ChaseBall["Execute"] = function(player)

	--PrintLuaMsg( player:ID() .. " State_ChaseBall Execute 123 " );

	PrintLuaMsg( player:ID() .. " State_ChaseBall Execute " );

	if player:BallWithinKickingRange() then

		PrintLuaMsg( player:ID() .. " State_ChaseBall GlobalPlayer change to State_KickBall 21" );

		player:GetFSM():ChangeState(State_KickBall)

		return;

	elseif  player:isClosestTeamMemberToBall() then

		PrintLuaMsg( player:ID() .. " State_ChaseBall Execute 345 " );

		player:SetSteeringTarget( player:BallPos() )

		return ;

	end

	PrintLuaMsg( player:ID() .. " State_ChaseBall GlobalPlayer change to State_ReturnGoHome 20" );

	player:GetFSM():ChangeState(State_ReturnGoHome);

end

State_ChaseBall["OnMessage"] = function(player,Msg)

	player:SetScriptValue( 0 );
	return;

end


State_ChaseBall["Exit"] = function(player)

	player:Steering():SeekOff();

end


State_SupportAttacker = { }

State_SupportAttacker["OnMessage"] = function(player,Msg)

	player:SetScriptValue( 0 );
	return;

end


State_SupportAttacker["Enter"] = function(player)

	player:Steering():ArriveOn();
	player:SetSteeringTarget( player:Team():GetSupportSpot() )

end


State_SupportAttacker["Execute"] = function(player)

	PrintLuaMsg( player:ID() .. " State_SupportAttacker Execute " );

	if not player:IsChaseBall() then
		return;
	end

	if  not player:Team():IsControl() then

		PrintLuaMsg( player:ID() .. " State_SupportAttacker GlobalPlayer change to State_ReturnGoHome 19" );
		
		player:GetFSM():ChangeState(State_ReturnGoHome);

	end

	--if  player:Team():GetSupportSpot()  ~=  player:Steering():Target()  then

	--	player:SetSteeringTarget(player:Team():GetSupportSpot() );
	--	player:Steering():ArriveOn();
	--end

	local  Vecs = Vector2D();

	if  player:Team():CanShootGoal(player:Pos(),6,Vecs) then

		player:Team():RequestPass(player)

	end

	if player:AtTarget() then

		player:Steering():ArriveOff();

		player:TrackBall();

		local  vec = Vector2D();
		player:SetVelocity(vec);


		if  not player:isThreatened() then

			player:Team():RequestPass(player)

		end

	end

end

State_SupportAttacker["Exit"] = function(player)

	player:Team():SetSupportingNULLPlayer();

	player:Steering():ArriveOff();

	PrintLuaMsg( player:ID() .. " Exit State_SupportAttacker! " );

end



State_ReturnGoHome = {}


State_ReturnGoHome["OnMessage"] = function(player,Msg)

	player:SetScriptValue( 0 );
	return;

end


State_ReturnGoHome["Enter"] = function(player)

	player:Steering():ArriveOn();

	if not player:InsideHomeRegion( player:Steering():Target(), 0 ) then

		--PrintLuaMsg( player:ID() .. " State_ReturnGoHome Enter " );

		player:SetSteeringTarget( player:GetHomeCenter() );

		return;
	end

end


State_ReturnGoHome["Execute"] = function(player)

	--PrintLuaMsg( player:ID() .. " State_ReturnGoHome Execute " );

	if  not player:IsChaseBall() then

		return;

	end

	
	PrintLuaMsg( player:ID() .. " State_ReturnGoHome Execute 111x" );

	if  player:Pitch():GameOn() then

		if player:isClosestTeamMemberToBall() and  not player:Pitch():GoalKeeperHasBall() then

			PrintLuaMsg( player:ID() .. " State_ReturnGoHome GlobalPlayer change to State_ChaseBall 18" );

			player:GetFSM():ChangeState(State_ChaseBall);

		end

	end
	
	PrintLuaMsg( player:ID() .. " State_ReturnGoHome Execute 222x" );

	if player:Pitch():GameOn() and  player:InsideHomeRegion( player:Pos(), 1 ) then

		player:SetSteeringTarget( player:Pos() );

		PrintLuaMsg( player:ID() .. " State_ReturnGoHome GlobalPlayer change to State_Wait 17" );

		player:GetFSM():ChangeState(State_Wait);


	elseif not player:Pitch():GameOn() and player:AtTarget() then

		PrintLuaMsg( player:ID() .. " State_ReturnGoHome GlobalPlayer change to State_Wait 16" );

		player:GetFSM():ChangeState(State_Wait);


	end

end


State_ReturnGoHome["Exit"] = function(player)

	player:Steering():ArriveOff();

end


State_Wait = { }


State_Wait["OnMessage"] = function(player,Msg)

	player:SetScriptValue( 0 );
	return;

end

State_Wait["Enter"] = function(player)

	if  not player:Pitch():GameOn()	then

		player:SetSteeringTarget( player:GetHomeCenter() )

	end

end



State_Wait["Execute"] = function(player)

	PrintLuaMsg( player:ID() .. " State_Wait Execute " );

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

				PrintLuaMsg( player:ID() .. " RequestPass Ball() " );

				player:Team():RequestPass(player);

				return;
	end


	if  player:Pitch():GameOn() then

		if player:isClosestTeamMemberToBall() and player:Team():IsReceiver() and  not player:Pitch():GoalKeeperHasBall() then

			--PrintLuaMsg( player:ID() .. " State_Wait GlobalPlayer change to State_ChaseBall 15" );

			player:GetFSM():ChangeState( State_ChaseBall )

			return ;

		end

		if not player:IsSelfRegin() then

			if player:Team():IsControl() or player:FollowReturn() then

				player:SetSteeringTarget( player:GetHomeCenter() );

				--PrintLuaMsg( player:ID() .. " State_Wait GlobalPlayer change to State_ReturnGoHome 14" );

				player:GetFSM():ChangeState( State_ReturnGoHome );

			end

		end

		if not player:Team():IsControl() and player:FollowTarget() then

			player:SetSteeringTarget( player:BallPos() )

			--PrintLuaMsg( player:ID() .. " State_Wait GlobalPlayer change to State_FollowBall 13" );

			player:GetFSM():ChangeState( State_FollowBall )

		end

	end


end

State_Wait["Exit"] = function(player)


end



State_FollowBall = { }


State_FollowBall["OnMessage"] = function(player,Msg)

	player:SetScriptValue( 0 );
	return;

end

State_FollowBall["Enter"] = function(player)

	player:Steering():SeekOn();

end

State_FollowBall["Execute"] = function(player)

	PrintLuaMsg( player:ID() .. " State_FollowBall Execute " );

	if not player:IsChaseBall() then

		return;

	end

	if player:Pitch():GameOn() then

		if  player:Team():IsControl() or player:FollowReturn() then

			player:SetSteeringTarget( player:GetHomeCenter() )
			--PrintLuaMsg( player:ID() .. " State_FollowBall GlobalPlayer change to State_ReturnGoHome 12" );
			player:GetFSM():ChangeState( State_ReturnGoHome )
		end

		if not player:isClosestTeamMemberToBall() and  not player:Team():IsControl() then

			player:SetSteeringTarget( player:BallPos() );

		end

		if player:isClosestTeamMemberToBall() and player:Team():IsThrowIn() then

			--PrintLuaMsg( player:ID() .. " State_FollowBall GlobalPlayer change to State_ChaseBall 11" );
			player:GetFSM():ChangeState( State_ChaseBall );

		end

		if player:isClosestTeamMemberToBall() and player:BallWithinReceivingRange() then

			--PrintLuaMsg( player:ID() .. " State_FollowBall GlobalPlayer change to State_ChaseBall 10" );

			player:GetFSM():ChangeState( State_ChaseBall );
		end

	end

end


State_FollowBall["Exit"] = function(player)

	player:Steering():SeekOff();

end



State_KickBall = { }


State_KickBall["OnMessage"] = function(player,Msg)

	player:SetScriptValue( 0 );
	return;

end

State_KickBall["Enter"] = function(player)

	player:Team():SetControllingPlayer( player );

	if not player:isReadyForNextKick() then

		PrintLuaMsg( player:ID() .. " State_KickBall GlobalPlayer change to State_ChaseBall 9" );
		player:GetFSM():ChangeState( State_ChaseBall )

	end
end


State_KickBall["Execute"] = function(player)

	PrintLuaMsg( player:ID() .. " State_KickBall Execute " );

	if not player:IsChaseBall() then

		PrintLuaMsg( player:ID() .. " State_KickBall GlobalPlayer change to State_Wait 8" );

		player:GetFSM():ChangeState( State_Wait )

	end

	PrintLuaMsg( player:ID() .. " State_KickBall player msg info 333" );

	local ToBall = Vec2DSub( player:BallPos(), player:Pos());

	local Vec2D  = Vec2DNormalize(ToBall);

	local dot    = player:Heading():Dot(Vec2D);

	if  player:Team():IsReceiver() or player:Pitch():GoalKeeperHasBall() then

		PrintLuaMsg( player:ID() .. " State_KickBall GlobalPlayer change to State_ChaseBall 7" );

		player:GetFSM():ChangeState( State_ChaseBall );

		return;
	end

	PrintLuaMsg( player:ID() .. " State_KickBall player msg info 222" );

	local  BallTarget = Vector2D();

	local  power = 6 * dot;

	if player:Team():CanShootGoal( player:BallPos() , power , BallTarget ) or  RandFloat() <  0.05 then

		BallTarget = AddNoiseToKick( player:BallPos() , BallTarget );

		local  kickDirection = Vec2DSub(BallTarget,player:BallPos());

		player:Ball():Kick( kickDirection , power , player );

		--PrintLuaMsg( player:ID() .. " State_KickBall GlobalPlayer change to State_Wait 6" );

		player:GetFSM():ChangeState( State_Wait )

		player:FindSupport();

		player:Team():SetThrow( 0 );

	end


	local receiver;

	power = 3 * dot;

	PrintLuaMsg( player:ID() .. " State_KickBall player FindPass! " );

	if player:isThreatened() then

		receiver = player:Team():FindRecvPass( player,BallTarget,power,100 );

		if receiver == 0 then

			return;
		end

		PrintLuaMsg(receiver .." receiver id " );

		BallTarget = AddNoiseToKick( player:BallPos() , BallTarget );

		local  kickDirection = Vec2DSub( BallTarget , player:BallPos() );

		player:Ball():Kick( kickDirection , power , player );

		player:Team():SetThrow( 0 );

		--local  addrvec    = VecAddr( BallTarget );

		--local  Dispatcher = MsgDispatcher();

		DispatchVctMessage(player:ID() , receiver , 0 , BallTarget );

		-- 传球消息公告

		player:GetFSM():ChangeState( State_Wait )

		player:FindSupport();

	else

		player:FindSupport();

		PrintLuaMsg( player:ID() .. " State_KickBall GlobalPlayer change to State_Dribble 4" );

		player:GetFSM():ChangeState( State_Dribble )

	end
	--local


end


State_KickBall["Exit"] = function(player)

	--PrintLuaMsg( player:ID() .. " leave State_KickBall " );

	if not player:isReadyForNextKick() then

		PrintLuaMsg( player:ID() .. " State_KickBall GlobalPlayer change to State_ChaseBall 3" );

		player:GetFSM():ChangeState( State_ChaseBall )

	end

end

State_Dribble = {}

State_Dribble["OnMessage"] = function(player,Msg)

	player:SetScriptValue( 0 );
	return;

end

State_Dribble["Enter"] = function(player)

	if not player:IsChaseBall() then
		return ;
	end

	player:Team():SetControllingPlayer( player )

end


State_Dribble["Execute"] = function(player)

	PrintLuaMsg( player:ID() .. " State_Dribble Execute " );

	if not player:IsChaseBall() then
		return ;
	end


	--local  dot = player:Team():HomeGoalFacing():Dot( player:Heading() );

	--if  dot < 0 then

	--	local  direction = player:Heading();

	--	local  sSign = player:Team():HomeGoalFacing():Sign( player:Heading() ) ;

	--	local  angle = 3.1415926/4 * -1 * sSign;

	--	Vec2DRotateAroundOrigin( direction , angle );

	--	local  KickingForce = 0.8;

	--	player:Ball():Kick( direction , KickingForce , player );

	--	player:Team():SetThrow(0);

	--else

		player:Ball():Kick( player:Team():HomeGoalFacing() , 1.5 , player );

		player:Team():SetThrow(0);

	--end

	PrintLuaMsg( player:ID() .. " State_Dribble GlobalPlayer change to State_ChaseBall 1112" );

	player:GetFSM():ChangeState( State_ChaseBall )


end


State_Dribble["Exit"] = function(player)

end


State_ReceiveBall  = { }

State_ReceiveBall["OnMessage"] = function(player,Msg)

	player:SetScriptValue( 0 );
	return;

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

	PrintLuaMsg( player:ID() .. " State_ReceiveBall Execute zxzasd23333" );

	if not player:IsChaseBall() then

		return;
	end

	PrintLuaMsg( player:ID() .. " State_ReceiveBall GlobalPlayer change to State_ChaseBall ,,,,,,,,,," );

	if  player:BallWithinReceivingRange() or not player:Team():IsControl() then

		PrintLuaMsg( player:ID() .. " State_ReceiveBall GlobalPlayer change to State_ChaseBall 111" );

		player:GetFSM():ChangeState( State_ChaseBall );

		return;
	end

	PrintLuaMsg( player:ID() .. " State_ReceiveBall GlobalPlayer change to State_ChaseBall zzzzzzzz" );

	if player:Steering():PursuitOn() then

		player:SetSteeringTarget( player:BallPos() );

	end

	if player:AtTarget() then

		player:Steering():ArriveOff();
		player:Steering():PursuitOff();

		player:TrackBall();
		local  vec = Vector2D();
		player:SetVelocity(vec);

	end

end


State_ReceiveBall["Exit"] = function(player)

	player:Steering():ArriveOff();
	player:Steering():PursuitOff();

	player:Team():SetNullReceiver();

end
