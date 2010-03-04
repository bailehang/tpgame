/** 
* @brief: 一个包括定时器结点的双向链表
 *
 * @Author:Expter
 * @date:  03/01/2010
 */

#pragma once

#include "TypeDef.h"
#include "tools.h"

/// 定时器指针结点
struct ListNode
{
	ListNode *next,*prev;
};

/// 
/// 定时器结点,tlist表示结点的指针,expires循环周期时间
/// etime 触发周期时间,pFun触发函数.
/// 
struct timernode
{
	ListNode  tlist;
	
	ulong    expires;
	
	ulong    etime;

	void     *pFun;
};


///			  
/// 一个轮子，一个双向链表
/// 
class CLinkList
{

public:

	CLinkList(void);

	CLinkList( int size );
	
	~CLinkList(void);
	
	/// 
	/// 初始化
	/// 
	void  init();

	/// 
	/// 让指针指向自己
	/// 
	void  init_list_self( int  index);

	/// 
	/// 把news插入到prev,next之间
	/// 
	void  insert_listnode(ListNode *news , ListNode* prev , ListNode* next);

	/// 
	/// 插入到链表头
	/// 
	void  insert_head( ListNode* news , ListNode* head );

	///
	/// 插入到链表尾
	/// 
	void  insert_tail( ListNode* news , ListNode* head );

	/// 
	/// 删除节点
	/// 
	void  list_del( ListNode* list);

	/// 
	/// 得到改轮子转到第几个插槽
	///
	int        GetIndex( ) const { return m_index ;}

	///
	/// 更新轮子的插槽
	///
	void       SetIndex(int idx) { m_index = idx  ;}

	/// 
	/// 得到轮子插槽的指针 
	///
	ListNode*  GetNode(int index) const;

private:
	///
	/// 轮子的插槽数组
	/// 
	ListNode *m_List;  
	
	///
	/// 轮子的索引
	/// 
	int		  m_index; 

	///
	/// 轮子大小
	///
	int       m_Size;

};
