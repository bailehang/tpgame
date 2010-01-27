// LinuxTimer.cpp : 定义控制台应用程序的入口点。
//
/** 
 * http://www.linuxforum.net/forum/showthreaded.php?Cat=&Board=driver&Number=385224&page=0&view=collapsed&sb=5&o=all&vc=1
 */
#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <list>

using namespace std;

#define TBITS1	6
#define	TBITS2	8
#define TSIZE1  ( 1 << TBITS1)
#define TSIZE2  ( 1 << TBITS2)
#define	TMASK1	( TSIZE1 - 1 )
#define TMASK2  ( TSIZE2 - 1 )

struct list_head 
{
	std::list<long>	list_t;
};

struct timer_list { 
	struct list_head list; 
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

static struct timer_vec * const tvecs[] = { 
	(struct timer_vec *)&tv1, &tv2, &tv3, &tv4, &tv5 
}; 

void init_timervecs (void) 
{ 
	int i; 

	/*
	for (i = 0; i < TVN_SIZE; i++) { 
		INIT_LIST_HEAD(tv5.vec + i); 
		INIT_LIST_HEAD(tv4.vec + i); 
		INIT_LIST_HEAD(tv3.vec + i); 
		INIT_LIST_HEAD(tv2.vec + i); 
	} 
	for (i = 0; i < TVR_SIZE; i++) 
		INIT_LIST_HEAD(tv1.vec + i); 
	*/
} 

static inline void internal_add_timer(struct timer_list *timer) 
{ 
	/* 
	* must be cli-ed when calling this 
	*/ 
	unsigned long expires = timer->expires; 
	unsigned long idx = (expires - m_jeffies)/100;// 精确到0.1秒 
	struct list_head * vec; 

	if (idx < TSIZE2 ) { 
		int i = expires & TMASK2; 
		vec = tv1.vec + i; 
	} else if (idx < 1 << (TBITS2 + TBITS1)) { 
		int i = (expires >> TBITS2) & TMASK1; 
		vec = tv2.vec + i; 
	} else if (idx < 1 << (TBITS2 + 2 * TBITS1)) { 
		int i = (expires >> (TBITS2 + TBITS1)) & TMASK1; 
		vec = tv3.vec + i; 
	} else if (idx < 1 << (TBITS2 + 3 * TBITS1)) { 
		int i = (expires >> (TBITS2 + 2 * TBITS1)) & TMASK1; 
		vec = tv4.vec + i; 
	} else if ((signed long) idx < 0) { 
		/* can happen if you add a timer with expires == jiffies, 
		* or you set a timer to go off in the past 
		*/ 
		vec = tv1.vec + tv1.index; 
	} else if (idx <= 0xffffffffUL) { 
		int i = (expires >> (TBITS2 + 3 * TBITS1)) & TMASK1; 
		vec = tv5.vec + i; 
 	} 

	// INIT_LIST_HEAD(&timer->list); 
}

int _tmain(int argc, _TCHAR* argv[])
{
	m_jeffies = time( NULL );

	
	
	return 0;
}

