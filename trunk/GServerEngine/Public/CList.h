/*
*	File  :  CList.h
*   Brief :  实现一个简单双向链表类
*   Author:  Expter 
*	Creat Date:  [2009/11/2]
*/

#pragma once

template<class T>
class TLinkedList
{
public:
	int	 Size;

	T	*Head;
	T	*Tail;

	TLinkedList()
	{
		Head = Tail = NULL;		
		Size = 0;
	}

	~TLinkedList(){ ReleaseList(); }

	/*
	*	链表的Get操作
	*/
	T*  GetHead(){ return Head; }

	T*	GetTail(){ return Tail; }

	T*  GetNext(T* Node){ return Node->next;}

	T*	GetPrev(T* Node){ return Node->prev;}

	int	GetSize() { return Size; }

	/*
	*  Add , Insert ,Delete操作
	*/
	void AddNode(T* Node)
	{
		Node->next = Node->prev = NULL;

		if(this->Head == NULL)			 
		{
			this->Head = Node;
			this->Tail = Node;

			this->Size = 1;
		}
		else
		{
			this->Tail->next = Node;
			Node->prev = this->Tail;
			this->Tail = Node;

			this->Size++;
		}	
	}

	void AddNodeToHead(T* Node)		
	{
		Node->next = Node->prev = NULL;

		if(this->Head == NULL)			 
		{
			this->Head = Node;
			this->Tail = Node;

			this->Size = 1;
		}
		else
		{
			this->Head->prev = Node;
			Node->next = this->Head;
			this->Head = Node;

			this->Size++;
		}
	}

	void InsertNode(T* Find_Node,T* Insert_Node)
	{
		Insert_Node->next = Insert_Node->prev = NULL;

		if(Find_Node->next != NULL)      
		{
			T temp;

			temp.next = Find_Node->next;
			Find_Node->next = Insert_Node;

			Insert_Node->next = temp.next;
			Insert_Node->prev = Find_Node;
			temp.next->prev = Insert_Node;
		}
		else 
		{
			Insert_Node->prev = Find_Node;
			Find_Node->next = Insert_Node;

			this->Tail = Insert_Node;
		}

		this->Size++; 
	}

	void RemoveNode(T* Del_Node)
	{  
		// head point
		if(Del_Node->prev == NULL)
		{
			if(Del_Node->next != NULL)
			{
				this->Head = Del_Node->next;
				this->Head->prev = NULL;

				this->Size--;
			}
			else
			{
				this->Head = this->Tail = NULL;

				this->Size = 0;
			}
		}
		else
		{
			if(Del_Node->next != NULL)
			{
				Del_Node->prev->next = Del_Node->next;
				Del_Node->next->prev = Del_Node->prev;

				this->Size--;
			}
			else
			{
				Tail = Del_Node->prev;
				Tail->next = NULL;

				this->Size--;
			}
		}
		Del_Node->prev = Del_Node->next = NULL;
	}

	void ReleaseList()
	{
		if(this->Head)
		{
			T* temp1=this->Head;
			T* temp2;

			while(temp1)
			{
				temp2 = temp1->next;
				RemoveNode(temp1);
				temp1 = temp2;
			}
		}
	}	

};
