// LinuxTimer.cpp : 定义控制台应用程序的入口点。
//

/** 
 * http://www.linuxforum.net/forum/showthreaded.php?Cat=&Board=driver&Number=385224&page=0&view=collapsed&sb=5&o=all&vc=1
 */

#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <cstdlib>
#include <windows.h>
using namespace std;

long  Ssum = 0;
void fun1( unsigned long v)
{
	std::cout << " Summ = " << Ssum << " fun1  v= " << v << std::endl; 
}

void fun2( unsigned long v)
{
	std::cout << " Summ = " << Ssum << " fun2  v= " << v << std::endl; 
}

void fun3( unsigned long v)
{
	std::cout << " Summ = " << Ssum << " fun3  v= " << v << std::endl; 
}

#define TBITS1	6
#define	TBITS2	8
#define TSIZE1  ( 1 << TBITS1)
#define TSIZE2  ( 1 << TBITS1)
#define	TMASK1	( TSIZE1 - 1 )
#define TMASK2  ( TSIZE2 - 1 )

#define POISON_POINTER_DELTA 0
#define LIST_POISON1  ((void *)0x00100100 + POISON_POINTER_DELTA)
#define LIST_POISON2  ((void *)0x00200200 + POISON_POINTER_DELTA)

#define container_of(ptr, type, member) ({   \
			})

#define list_entry(ptr, type, member) \
		container_of( ptr , type , member )

struct list_head 
{
	list_head* next;
	list_head* prev;
};

struct timer_list { 
	struct list_head list_t; 
	unsigned long expires; 
	unsigned long data; 
	void (*function)(unsigned long); 
}; 

struct timer_vec 
{
	int	index;
	list_head  vec[TSIZE1];
};

struct timer_vec_root 
{
	int	index;
	list_head  vec[TSIZE2];
};


static	long	m_jeffies = 0 ;
static  struct  timer_vec tv5; 
static  struct  timer_vec tv4; 
static  struct  timer_vec tv3; 
static  struct  timer_vec tv2; 
static  struct  timer_vec_root tv1; 

static struct timer_vec* const tvecs[] = { 
	(struct timer_vec *)&tv1, &tv2, &tv3, &tv4, &tv5 
}; 

#define NOOF_TVECS (sizeof(tvecs) / sizeof(tvecs[0])) 

inline void	 INIT_LIST_HEAD( struct list_head* _list )
{
	_list->next = _list;
	_list->prev = _list;
}

inline void	list_add(struct list_head* news ,  struct list_head* prev , struct list_head* next)
{
	next->prev = news;
	news->next = next;
	news->prev = prev;
	prev->next = news;
}

inline void list_add_head( struct list_head* news , struct list_head* head)
{
	list_add( news , head->prev , head );
}

void init_timervecs (void) 
{ 
	int i; 	
	for (i = 0; i < TSIZE1; i++) { 
		INIT_LIST_HEAD(tv5.vec + i); 
		INIT_LIST_HEAD(tv4.vec + i); 
		INIT_LIST_HEAD(tv3.vec + i); 
		INIT_LIST_HEAD(tv2.vec + i); 
	} 
	for (i = 0; i < TSIZE2; i++) 
		INIT_LIST_HEAD(tv1.vec + i); 
} 

static inline void internal_add_timer(struct timer_list *timer) 
{ 
	/* 
	* must be cli-ed when calling this 
	*/ 
	//unsigned long expires = timer->expires; 
	unsigned long expires = (timer->expires - m_jeffies) * 2 ;// 精确到0.1秒 
	struct list_head * vec; 

	if (expires < TSIZE2 ) { 
		int i = expires & TMASK2; 
		vec = tv1.vec + i; 
		//std::cout <<" i = " << i << endl;
	} else if (expires < 1 << (TBITS2 + TBITS1)) { 
		int i = (expires >> TBITS2) & TMASK1; 
		vec = tv2.vec + i; 
		//std::cout <<" i = "<< i <<"  "<< expires << std::endl;
	} else if (expires < 1 << (TBITS2 + 2 * TBITS1)) { 
		int i = (expires >> (TBITS2 + TBITS1)) & TMASK1; 
		vec = tv3.vec + i; 
	} else if (expires < 1 << (TBITS2 + 3 * TBITS1)) { 
		int i = (expires >> (TBITS2 + 2 * TBITS1)) & TMASK1; 
		vec = tv4.vec + i; 
	} else if ((signed long) expires < 0) { 
		/* can happen if you add a timer with expires == jiffies, 
		* or you set a timer to go off in the past 
		*/ 
		vec = tv1.vec + tv1.index; 
	} else if (expires <= 0xffffffffUL) { 
		int i = (expires >> (TBITS2 + 3 * TBITS1)) & TMASK1; 
		vec = tv5.vec + i; 
 	} 

	list_add_head(&timer->list_t , vec );
}

static inline int timer_pending (const struct timer_list * timer) 
{ 
	return timer->list_t.next != NULL; 
} 

static inline void init_timer(struct timer_list * timer) 
{ 
	timer->list_t.next = timer->list_t.prev = NULL; 
} 

static inline	void list_del( struct list_head* prev , struct list_head* next )
{
	next->prev = prev;
	prev->next = next;
}

static inline	void list_del( struct list_head* entity)
{
	list_del( entity->prev , entity->next );
	// entity->next = LIST_POISON1;
	// entity->prev = LIST_POISON2;
}

static inline int detach_timer (struct timer_list *timer) 
{ 
	if (!timer_pending(timer)) 
		return 0; 
	list_del(&timer->list_t); 
	return 1; 
} 

int del_timer(struct timer_list * timer) 
{ 
	int ret; 
	unsigned long flags; 

	ret = detach_timer(timer); 
	timer->list_t.next = timer->list_t.prev = NULL; 

	return ret; 
} 

static inline void cascade_timers(struct timer_vec *tv) 
{ 
	/* cascade all the timers from tv up one level */ 
	struct list_head *head, *curr, *next; 

	head = tv->vec + tv->index; 
	curr = head->next; 
	/* 
	* We are removing _all_ timers from the list_t, so we don't have to 
	* detach them individually, just clear the list_t afterwards. 
	*/ 
	while (curr != head) { 
		struct timer_list *tmp; 

		//(((timer_list *)0)->list_t)* mptr = curr; 
		tmp = (timer_list *)( (char *)curr - offsetof(timer_list,list_t) );

		//tmp = list_entry(curr, struct timer_list, list_t); 
		next = curr->next; 
		list_del(curr); // not needed 
		internal_add_timer(tmp); 
		curr = next; 
		//std::cout <<" v2 -> v1 " << std::endl;
	} 
	INIT_LIST_HEAD(head); 
	tv->index = (tv->index + 1) & TMASK1; 
} 

void add_timer(struct timer_list *timer) 
{ 
	unsigned long flags; 

	if (timer_pending(timer)) 
		goto bug; 
	internal_add_timer(timer); 
	return; 
bug: 
	printf("bug: kernel timer added twice at \n"); 
} 

static inline void run_timer_list(long jiffies ) 
{ 
	while ( jiffies- m_jeffies >= 0) { 
		struct list_head *head, *curr; 
		if (!tv1.index) { 
			int n = 1; 
			do { 
				cascade_timers(tvecs[n]); 
			} while (tvecs[n]->index == 1 && ++n < NOOF_TVECS); 
		 } 
repeat: 
		head = tv1.vec + tv1.index; 
		curr = head->next; 
		if (curr != head) { 
			struct timer_list *timer; 
			void (*fn)(unsigned long); 
			unsigned long data; 

			timer = (timer_list *)( (char *)curr - offsetof(timer_list,list_t) );
			//list_entry(curr, struct timer_list, list_t); 
			fn = timer->function; 
			data= timer->data; 

			detach_timer(timer); 
			timer->list_t.next = timer->list_t.prev = NULL; 

			/// 超时触发
			//timer_enter(timer); 
			fn(data); 
			//timer_exit(); 

			Ssum ++ ;
			goto repeat; 
		} 
		++m_jeffies; 
		tv1.index = (tv1.index + 1) & TMASK1; 
	} 
} 


int mod_timer(struct timer_list *timer, unsigned long expires) 
{ 
	int ret; 
	unsigned long flags; 

	timer->expires = expires; 
	ret = detach_timer(timer); 
	internal_add_timer(timer); 
	return ret; 
} 

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));
	m_jeffies = time( NULL );

	void (*f1)(unsigned long);
	void (*f2)(unsigned long);
	void (*f3)(unsigned long);

	f1 = &fun1;
	f2 = &fun2;
	f3 = &fun3;

	init_timervecs();

	//std::cout << m_jeffies << std::endl;
	for ( int i = 0 ; i < 1000 ; i++ )
	{
		timer_list *tm = new timer_list();
		tm->expires = time(NULL) + rand() % 50 ;
		tm->data    = rand() % 10;

		if ( tm->expires % 3 ==  0 )
			tm->function = f1;
		else if ( tm->expires % 3 == 1)
		{
			tm->function = f2;
		}
		else if ( tm->expires % 3 == 2)
		{
			tm->function = f3;
		}

		//std::cout << tm->expires << std::endl;
		add_timer( tm );
		//Sleep( 1 );
		//std::cout << i << std::endl;
	}

	std::cout << time(NULL) - m_jeffies << std::endl;
	for ( ;; )
	{
		run_timer_list( time(NULL) );
	}
	return 0;
}

