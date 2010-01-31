#pragma  once

// 通过模板求偏移地址
template < class T , typename type>
long   mem_offset( type  T::* ptr)
{
	return  *(type*)&ptr;
}

#define mem_offsetof(c,type,member)  mem_offset<c,type>(&c::member)
// mem_offsetof(classa,int,b)  
// ---- mem_offset<classa,int>(&classa::b);

// 通过宏求偏移地址

// #define offsetof(s,m)   (size_t)&reinterpret_cast<const volatile char&>((((s *)0)->m))

#define offsetof(s,m)   (size_t)(char*)&((s*)0->m)