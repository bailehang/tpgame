/** 
 * @brief: 一个双向链表
 *
 * @Author:Expter
 * @date:  03/01/2010
 */

#pragma once

#include "TypeDef.h"
#include "tools.h"

struct ListNode
{
	ListNode *next,*prev;
};

struct timernode
{
	ListNode  tlist;
	/// 循环周期时间
	ulong    expires;
	/// 触发周期时间
	ulong    etime;
	/// 触发函数
	void     *pFun;
};

class CLinkList
{

public:

	CLinkList(void);

	CLinkList( int size );
	
	~CLinkList(void);
	
	/// 初始化
	void  init();

	/// 让指针指向自己
	void  init_list_self( int  index);

	/// 把news插入到prev,next之间
	void  insert_listnode(ListNode *news , ListNode* prev , ListNode* next);

	/// 插入到链表头
	void  insert_head( ListNode* news , ListNode* head );

	/// 插入到链表尾
	void  insert_tail( ListNode* news , ListNode* head );

	/// 删除节点
	void  list_del( ListNode* list);

	int        GetIndex( ) const { return m_index ;}

	void       SetIndex(int idx) { m_index = idx  ;}

	ListNode*  GetNode(int index) const;

private:

	ListNode *m_List;  /// 链表

	int		  m_index; /// 索引

	int       m_Size;

};
