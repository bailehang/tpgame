#pragma once

class CBaseEntity
{
public:
	CBaseEntity( int id )
	{
		SetId( id );
	}

	virtual ~CBaseEntity(void);

	virtual  void Update() = 0;

	int		 GetId()const 	{	return m_id ; }

private:
	static	long		m_iNextValidId;
	
	int					m_id;

	void	SetId( int val );
};
