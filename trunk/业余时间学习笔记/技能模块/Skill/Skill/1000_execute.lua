
skill_1000_begin = function( skill , skillparam )
	
	local lvl =  skillparam:GetLevl();
	
	--Pk ok
	if skill:isAttacKOk() == 0 then
		
		--return 0
		send_error_msg( 5 );
		return 0;
	end	
	
	-- Attack type
	if skill:isAttackType() == 0 then 
	
		send_error_msg( 5 );
		return 0
	end
	
	-- Attack len
	if skill:isAttackLen() == 0 then
	
		send_error_msg( 6 );
		return 0	
	end
	
	-- Attack property
	if skill:isAttackPro() == 0 then
		
		send_error_msg( 10 );	
		return 0
	end
	
end