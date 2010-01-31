#pragma  once

#include "State.h"
#include "../singleton.h"

#define  CreateStateSet( StateName )	\
		 GetInst(StateName)

template<class class_value>
class  CStateSet
{
	typedef  class_value	Ctype;
	typedef  std::map< eStateAi , Ctype* >	eState;
	eState::iterator						Eiter;

public:
	// µ¥¼þ
	CStateSet&	Instance()
	{
		static CStateSet   CSet;
		return   CSet;
	}

	void		RegCStateSet( eStateAi&  e ,const  char*  StateName)
	{
		m_stateset.insert( std::make_pair( e , CreateStateSet(StateName)) );	
	}
	
	Ctype*		GetState( eStateAi& e)
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
	
	/// singleton
public:
	CStateSet();
	CStateSet(const CStateSet&);
	~CStateSet();
	CStateSet& operator= (const CStateSet&);
};