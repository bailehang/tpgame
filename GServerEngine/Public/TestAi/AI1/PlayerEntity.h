#pragma  once

#include "../../State.h"
#include "../../BaseEntity.h"

class  CBaseEntity;
class  State;

class   CPlayerEntity : public CBaseEntity
{
private:
	State*			m_pCurrentState;

	eState			m_State;

public:

	CPlayerEntity( int id );

	void	Update();

	void	ChangeState( State* new_state );

	eState	GetState()const  {	return m_State; }

};