/**
 *   @brief		关于内存池的一些定义
 *	 @date		2010.05.01
 *   @modify	2010.10.10
 */


#pragma  once 

#define  CHUNKSIZE			20*(1<<20)		// 20M
#define  MAXALLOCSIZE	    3*CHUNKSIZE		// 60M

#define  MaxSize			((1<<30)-1)     // 最大范围
#define  LvlRange			512
#define  SizeRange			5120			// 预分配为小于等于5120，超过则通过预分配策略
#define  LONGSIZE		    sizeof(long)

const  long  Range[ ]   = { 16,32,64,128,256,512,1024,2048,3072,4096,5120 };

#define  DEF_IMPL_FACTORY_FUN( classname, baseclassname) \
	static  baseclassname* create();\
	static  void  destory(baseclassname*p) 

#define  DEL_IMPL_FACTORY_FUN( classname, baseclassname) \
	baseclassname* classname::create(){ return new classname(); } \
	void  classname::destory(baseclassname*p)  { if(p){ delete p ; p=next;} }

#define  FAC_IMPL_FACTORY_FUN( classname, baseclassname) \
	static baseclassname* create(){ return new classname(); } \
	static void  destory(baseclassname*p)  { if(p){ delete p ; p=next;} }

#define LP_SNMP_0(pname) 
#define LP_SNMP_1(pname)  pname##1
#define LP_SNMP_2(pname)  LP_SNMP_1(pname), pname##2
#define LP_SNMP_3(pname)  LP_SNMP_2(pname), pname##3
#define LP_SNMP_4(pname)  LP_SNMP_3(pname), pname##4
#define LP_SNMP_5(pname)  LP_SNMP_4(pname), pname##5
#define LP_SNMP_6(pname)  LP_SNMP_5(pname), pname##6
#define LP_SNMP_7(pname)  LP_SNMP_6(pname), pname##7
#define LP_SNMP_8(pname)  LP_SNMP_7(pname), pname##8
#define LP_SNMP_9(pname)  LP_SNMP_8(pname), pname##9
#define LP_SNMP_10(pname) LP_SNMP_9(pname), pname##10

#define DP_MTMP_0(type,name)
#define DP_MTMP_1(type,name) type##1 name##1
#define DP_MTMP_2(type,name) DP_MTMP_1(type,name), type##2 name##2
#define DP_MTMP_3(type,name) DP_MTMP_2(type,name), type##3 name##3
#define DP_MTMP_4(type,name) DP_MTMP_3(type,name), type##4 name##4
#define DP_MTMP_5(type,name) DP_MTMP_4(type,name), type##5 name##5
#define DP_MTMP_6(type,name) DP_MTMP_5(type,name), type##6 name##6
#define DP_MTMP_7(type,name) DP_MTMP_6(type,name), type##7 name##7
#define DP_MTMP_8(type,name) DP_MTMP_7(type,name), type##8 name##8
#define DP_MTMP_9(type,name) DP_MTMP_8(type,name), type##9 name##9
#define DP_MTMP_10(type,name) DP_MTMP_9(type,name), type##10 name##10

#define DP_STMP_0(type,pname)
#define DP_STMP_1(type,pname) type pname##1
#define DP_STMP_2(type,pname) DP_STMP_1(type,pname), type pname##2
#define DP_STMP_3(type,pname) DP_STMP_2(type,pname), type pname##3
#define DP_STMP_4(type,pname) DP_STMP_3(type,pname), type pname##4
#define DP_STMP_5(type,pname) DP_STMP_4(type,pname), type pname##5
#define DP_STMP_6(type,pname) DP_STMP_5(type,pname), type pname##6
#define DP_STMP_7(type,pname) DP_STMP_6(type,pname), type pname##7
#define DP_STMP_8(type,pname) DP_STMP_7(type,pname), type pname##8
#define DP_STMP_9(type,pname) DP_STMP_8(type,pname), type pname##9
#define DP_STMP_10(type,pname) DP_STMP_9(type,pname), type pname##10

template <class T> 
inline T * CALL_CON( T * ptMem ) 
{ 
	T * pt = new(ptMem)T; 
	return pt; 
} 

#define DEFINE_CALL_CON( paramcount ) template <class T, DP_STMP_##paramcount( typename, tp ) >\
	inline T * Alloc(DP_MTMP_##paramcount( tp, p ) ){\
	unsigned long lSize = sizeof(T);\
	void* ptMem = Alloc(lSize);\
	if( !ptMem) return NULL; \
	T * pt = new(ptMem)T( LP_SNMP_##paramcount( p ) );\
	return pt;\
}