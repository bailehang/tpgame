/*
*	File  :	 
*   Brief :  消息队列
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "PacketManager.h"

class CMsgQueue
{
	typedef  CQueue<PACKETBUFFER>  msgQueue;

public:
	/*
	 *  Brief: 构造函数，初始化操作
	 */
	CMsgQueue(void);

	~CMsgQueue(void);

	/*
	 *   Push数据
	 */
	bool	PushMessage(	PACKETBUFFER  *pMsg );

	/*
	 *   Pop数据
	 */
	PACKETBUFFER	*PopMessage(	);

	/*
	 *  取出消息列队size个数据
	 */
	void	GetSizeMessage(  msgQueue& pTempQMsg , int size );

	/*
	 *  取出所有数据
	 */
	void	GetAllMessage(	 msgQueue& pTempQMsg);

	/*
	 *	返回消息列队数据大小
	 */
	int		GetSize();

	/*
	 *  数据清空
	 */
	void	Clear();

private:
	msgQueue			m_msgQueue;			/// 消息队列
	sync::csectionlock	m_Section;			/// 锁

};
