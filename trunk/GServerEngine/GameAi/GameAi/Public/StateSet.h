#pragma  once

#include <map>

#include "BaseDef.h"
#include "State.h"
#include "../Entity/StatePro/DeadState.h"
#include "../Entity/StatePro/FightState.h"
#include "../Entity/StatePro/FleeState.h"
#include "../Entity/StatePro/FollowState.h"
#include "../Entity/StatePro/GoHomeState.h"
#include "../Entity/StatePro/PeaceState.h"
#include "../singleton.h"


class  CStateSet
{
	typedef  std::map< eStateAi , CState* >			eState;
	typedef  eState::iterator					    Eiter;

public:

	void       Create()
	{
		RegCStateSet( Ent_PeaceState , static_cast<CState*>(&GetInstObj(CPeaceState)));
		RegCStateSet( Ent_FleeState ,  static_cast<CState*>(&GetInstObj(CFleeState)) );
		RegCStateSet( Ent_FightState , static_cast<CState*>(&GetInstObj(CFightState)));
		RegCStateSet( Ent_FollowState, static_cast<CState*>(&GetInstObj(CFollowState)));
		RegCStateSet( Ent_DeadState  , static_cast<CState*>(&GetInstObj(CDeadState)) );
		RegCStateSet( Ent_GoHomeState, static_cast<CState*>(&GetInstObj(CGoHomeState)));
	}

	void		RegCStateSet( eStateAi  e , CState*  tp )
	{
		m_stateset.insert( std::make_pair( e , tp ) );
	}
	
	CState*		GetState( eStateAi e)
	{
		Eiter  itr = m_stateset.find ( e );
		if ( itr != m_stateset.end() )
		{
			return itr->second;
		}
		return  NULL;
	}

private:
	eState			m_stateset;

};