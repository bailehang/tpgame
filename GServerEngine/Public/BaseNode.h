/*
*	File  :  BaseNode.h
*   Brief :  一个结点
*   Author:  Expter 
*	Creat Date:  [2009/11/2]
*/

#pragma  once

template < typename T >

class  CBaseNode
{
public:

	T * next;
	T * prev;

	CBaseNode()
	{
		next = prev = NULL;
	} 

};