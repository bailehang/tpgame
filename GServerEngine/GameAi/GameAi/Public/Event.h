/*
 * File  :  Event.h
 * Brief :	Game AI Event Dispactch
 * Author:  Expter  
 * Date  :  [2010-1-23]
 *                             
 **/
#include "BaseDef.h"
#include "BaseGameEntity.h"

#include  <vector>
using namespace std;

class  CBaseGameEntity ;
class  CEventDispatch
{
private:
	std::vector<tagEevent>  m_EventList;

	void   DealEvent(const tagEevent&   Event);

public:
	CEventDispatch()	{	m_EventList.clear();	}
	~CEventDispatch()	{	m_EventList.clear();	}

	void	RegEvent(const tagEevent&   Event);

	/// 处理所有的事件
	void	DispacthEevent( );

};