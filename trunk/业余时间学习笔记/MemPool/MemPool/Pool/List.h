/** 
 *  file   list.h
 *  brief  封装一个双向链表
 *  author Expter
 *  
 *  date   2010/01/1
 */

#pragma  once 

template < class T>
class   TlinkedList
{
public:

	/// Constructor 
	TlinkedList(){  m_Head = m_Tail = NULL ; m_Size = 0; };
	/// Destructor
	~TlinkedList(){};


	/// the operation of linked list
public:
	T*		GetHead() { return  m_Head; }
	T*		GetTail() { return  m_Tail; }
	T*		GetNext(T* Node) { return Node->next;}
	T*		GetPrev(T* Node) { return Node->prev;}
	long    GetSize() { return  m_Size; }

	/// about list add,insert,remove function
	/// AddListToTail
	void    PushNode(T* Node)
	{
		Node->next = Node->prev = NULL;

		if ( this->m_Head == NULL )
		{
			this->m_Head = Node;
			this->m_Tail = Node;

			this->m_Size = 1;
		}
		else
		{
			this->m_Tail->next = Node;
			Node->prev  = this->m_Tail;
			this->m_Tail= Node;

			this->m_Size ++;
		}
	}

	/// Add Node to list Head
	void    PushNodeToHead(T* Node)
	{
		Node->next = Node->prev = NULL;
		
		if ( this->m_Head == NULL )
		{
			this->m_Head = Node;
			this->m_Tail = Node;
			this->m_Size = 1;
		}
		else 
		{
			this->m_Head->prev = Node;
			Node->next = this->m_Head;
			this->m_Head = Node;

			this->m_Size++;
		}
	}

	/// insert Insert_Node in Find_Node end
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

			this->m_Tail = Insert_Node;
		}

		this->m_Size++; 
	}

	void  RemoveNode( T*  DelNode)
	{
		if ( DelNode->prev == NULL )
		{
			if ( DelNode->next != NULL )
			{
				this->m_Head = DelNode->next;
				this->m_Head->prev = NULL;

				this->m_Size -- ;
			}
			else
			{
				this->m_Head = this->m_Tail = NULL;

				this->m_Size = 0;
			}
		}
		else
		{
			if ( DelNode->next != NULL )
			{
				DelNode->prev->next = DelNode->next;
				DelNode->next->prev = DelNode->prev;

				this->m_Size -- ;
			}
			else
			{
				m_Tail = DelNode->prev;
				m_Tail->next = NULL;

				this->m_Size -- ;
			}
		}
		DelNode->prev = DelNode->next = NULL;
	}

	void  ReleaseList()
	{
		if ( this->m_Head )
		{
			T* temp1 = this->m_Head;
			T* temp2 = NULL;

			while ( temp1 != NULL )
			{
				temp2 = temp1->next;
				RemoveNode( temp1 );
				temp1 = temp2;
			}
		}
	}


private:
	long    m_Size;
	T*		m_Head;
	T*		m_Tail;
};
