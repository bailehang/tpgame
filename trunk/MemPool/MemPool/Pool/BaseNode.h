/** 
 *
 *
 *
 */

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