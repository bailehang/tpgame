
#pragma once 

#include "../../Render/Vector2D.h"
#include "../../Render/Geometry.h"
#include "../../Render/Utils.h"
#include "../../Public/BaseDef.h"
#include "../Telegram.h"

class Telegram;

class CBaseEntity
{
	enum {default_entity_type = -1};

private:

	long	      m_id;
	ePlayerType	  m_itype;
	bool		  m_bTag;
	
	
protected:
	Vector2D	  m_vPosition;
	Vector2D	  m_vScal;

	double		  m_dBoundingRadius;
	static int    m_iNextValidID;
public:
	CBaseEntity( long  id);

	virtual   void  Update() = 0 ;

	virtual   void  Render() = 0 ;

	virtual	  bool  HandleEvent(const Telegram& e ){ return false;}

	virtual	  void  Write(std::ostream&  os)const{}
	virtual	  void  Read (std::ifstream& is){}

	static    int   GetNextValidId()	  { return m_iNextValidID; }
	static    void  ResetNextValidId()	  { m_iNextValidID = 0 ;   }

	Vector2D		Pos()const{return m_vPosition;}
	void			SetPos(Vector2D new_pos){m_vPosition = new_pos;}

	double			BRadius()const		  {return m_dBoundingRadius;}
	void			SetBRadius(double r)  {m_dBoundingRadius = r;}
	long			GetID()const		  {return m_id;}

	bool			IsTagged()const{return m_bTag;}
	void			Tag(){m_bTag = true;}
	void			UnTag(){m_bTag = false;}

	Vector2D		Scale()const{return m_vScal;}
	void			SetScale(Vector2D val)
	{
		m_dBoundingRadius *= MaxOf(val.x, val.y)/MaxOf(m_vScal.x, m_vScal.y); m_vScal = val;	
	}
	void			SetScale(double val)
	{	
		m_dBoundingRadius *= (val/MaxOf(m_vScal.x, m_vScal.y)); m_vScal = Vector2D(val, val);	
	} 

	ePlayerType    EntityType()const			{return m_itype;}
	void           SetEntityType(ePlayerType new_type){m_itype = new_type;}

	virtual	  ~CBaseEntity() {}
};