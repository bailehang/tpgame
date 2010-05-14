
#pragma  once 


enum  eType_Buffer
{
	eBT_32 =0,
	eBT_64,
	eBT_128,
	eBT_256,
	eBT_512,
	eBT_1024,
	eBT_2048,
	eBT_5120,

	eBT_Num,
};

#define  COM_BINE(a,b)  ( (a<<16) | b )

#define  DEF_IMPL_FACTORY_FUN( classname, baseclassname) \
	static  baseclassname* create();\
	static  void  destory(baseclassname*p) 

#define  DEL_IMPL_FACTORY_FUN( classname, baseclassname) \
	baseclassname* classname::create(){ return new classname(); } \
	void  classname::destory(baseclassname*p)  { if(p){ delete p ; p=next;} }

#define  FAC_IMPL_FACTORY_FUN( classname, baseclassname) \
	static baseclassname* create(){ return new classname(); } \
	static void  destory(baseclassname*p)  { if(p){ delete p ; p=next;} }

#define  BIND( id , classname ) \
	;

	
