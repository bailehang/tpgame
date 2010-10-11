1,防止一个头文件被重复包含 
#ifndef BODYDEF_H 
#define BODYDEF_H 
  //头文件内容 
#endif 
2,得到指定地址上的一个字节或字 
#define  MEM_B( x )  ( *( (byte *) (x) ) ) 
#define  MEM_W( x )  ( *( (word *) (x) ) ) 
3,得到一个field在结构体(struct)中的偏移量 
#define FPOS( type, field ) ( (dword) &(( type *) 0)-> field )
4,得到一个结构体中field所占用的字节数 
#define FSIZ( type, field ) sizeof( ((type *) 0)->field ) 
5,得到一个变量的地址（word宽度） 
#define  B_PTR( var )  ( (byte *) (void *) &(var) ) 
#define  W_PTR( var )  ( (word *) (void *) &(var) ) 
6,将一个字母转换为大写 
#define  UPCASE( c ) ( ((c) >= ''a'' && (c) <= ''z'') ? ((c) - 0x20) : (c) ) 
7,判断字符是不是10进值的数字 
#define  DECCHK( c ) ((c) >= ''0'' && (c) <= ''9'') 
8,判断字符是不是16进值的数字 
#define  HEXCHK( c ) ( ((c) >= ''0'' && (c) <= ''9'') ||((c) >= ''A'' && (c) <= ''F'') ||((c) >= ''a'' && (c) <= ''f'') ) 
9,防止溢出的一个方法 
#define  INC_SAT( val )  (val = ((val)+1 > (val)) ? (val)+1 : (val)) 
10,返回数组元素的个数 
#define  ARR_SIZE( a )  ( sizeof( (a) ) / sizeof( (a[0]) ) ) 
11,使用一些宏跟踪调试 
ANSI标准说明了五个预定义的宏名。它们是： 
_LINE_ (两个下划线)，对应%d
_FILE_     对应%s
_DATE_   对应%s
_TIME_    对应%s
_STDC_ 

 
宏中"#"和"##"的用法
我们使用#把宏参数变为一个字符串,用##把两个宏参数贴合在一起.
#define STR(s)     #s 
#define CONS(a,b)  int(a##e##b) 
Printf(STR(vck));           // 输出字符串"vck" 
printf("%d\n", CONS(2,3));  // 2e3 输出:2000 

当宏参数是另一个宏的时候 
需要注意的是凡宏定义里有用"#"或"##"的地方宏参数是不会再展开. 
#define A          (2) 
#define STR(s)     #s 
#define CONS(a,b)  int(a##e##b) 
printf("%s\n", CONS(A, A));               // compile error  
这一行则是： 
printf("%s\n", int(AeA)); 
INT_MAX和A都不会再被展开, 然而解决这个问题的方法很简单. 加多一层中间转换宏. 
加这层宏的用意是把所有宏的参数在这层里全部展开, 那么在转换宏里的那一个宏(_STR)就能得到正确的宏参数
#define STR(s)      _STR(s)          // 转换宏 
#define CONS(a,b)   _CONS(a,b)       // 转换宏 
printf("int max: %s\n", STR(INT_MAX));          // INT_MAX,int型的最大值,为一个变量 #include<climits> 
输出为: int max: 0x7fffffff 
STR(INT_MAX) -->  _STR(0x7fffffff) 然后再转换成字符串; 

printf("%d\n", CONS(A, A)); 
输出为：200 
CONS(A, A)  -->  _CONS((2), (2))  --> int((2)e(2)) 

"#"和"##"的一些应用特例 
1、合并匿名变量名 
#define  ___ANONYMOUS1(type, var, line)  type  var##line 
#define  __ANONYMOUS0(type, line)  ___ANONYMOUS1(type, _anonymous, line) 
#define  ANONYMOUS(type)  __ANONYMOUS0(type, __LINE__) 
例：ANONYMOUS(static int);  即: static int _anonymous70;  70表示该行行号; 
第一层：ANONYMOUS(static int);  -->  __ANONYMOUS0(static int, __LINE__); 
第二层：                        -->  ___ANONYMOUS1(static int, _anonymous, 70); 
第三层：                        -->  static int  _anonymous70; 
即每次只能解开当前层的宏,所以__LINE__在第二层才能被解开; 

2、填充结构 
#define  FILL(a)   {a, #a} 

enum IDD{OPEN, CLOSE}; 
typedef struct MSG{ 
  IDD id; 
  const char * msg; 
}MSG; 

MSG _msg[] = {FILL(OPEN), FILL(CLOSE)}; 
相当于： 
MSG _msg[] = {{OPEN, "OPEN"}, 
              {CLOSE, "CLOSE"}}; 

3、记录文件名 
#define  _GET_FILE_NAME(f)   #f 
#define  GET_FILE_NAME(f)    _GET_FILE_NAME(f) 
static char  FILE_NAME[] = GET_FILE_NAME(__FILE__); 

4、得到一个数值类型所对应的字符串缓冲大小 
#define  _TYPE_BUF_SIZE(type)  sizeof #type 
#define  TYPE_BUF_SIZE(type)   _TYPE_BUF_SIZE(type) 
char     buf[TYPE_BUF_SIZE(INT_MAX)]; 
     -->  char  buf[_TYPE_BUF_SIZE(0x7fffffff)]; 
     -->  char  buf[sizeof "0x7fffffff"]; 
这里相当于： 
char  buf[11]; 
