State_ChaseBall = {}

State_ChaseBall["Enter"] = function(player)

	PrintLuaMsg( player:ID() .. " Enter State_ChaseBall " );

end

State_ChaseBall["Execute"] = function(player)

	PrintLuaMsg( player:ID() .. " State_ChaseBall Execute " );
	
	if  player:ID() > 100 then 
		
		PrintLuaMsg( player:ID() .. " State_ChaseBall GlobalPlayer change to State_ReturnGoHome" );

		player:GetFSM():ChangeState(State_ReturnGoHome);
	end

end

State_ChaseBall["OnMessage"] = function(player,Msg)

	return 0;
end


State_ChaseBall["Exit"] = function(player)

	PrintLuaMsg( player:ID() .. "Leave State_ChaseBall! " );
end

