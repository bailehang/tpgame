/// 
///  file  BaseNode.h
///  brief 一个双向链表需要的前后指针节点
/// 

#pragma  once 

template < typename  T >
class  CNode
{
	typedef  T  Node;

public:

	Node*	next;
	Node*   prev;

	CNode()
	{
		next = prev = NULL;
	}
};