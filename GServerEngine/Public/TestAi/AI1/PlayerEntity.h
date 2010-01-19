#pragma  once

#include "../../State.h"
#include "../../BaseEntity.h"

class  CBaseEntity;
class  State;

class   CPlayerEntity : public CBaseEntity
{
private:
	State*			m_pCurrentState;

	eAction			m_Local;

public:

	CPlayerEntity( int id );

	void	Update();

	void	ChangeState( State* new_state );

	eAction GetAction()const  {	return m_Local; }

};