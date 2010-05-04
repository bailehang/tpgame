#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdarg.h> 

using namespace std;

void ArgTest(int num,...)
{
     va_list argp;
     va_start(argp,num);
     
     while(--num>=0)
     {
          printf("%s",(va_arg(argp,char*)));
     }
     va_end(argp);
}

void error( char *fmt, ... ) 
{
      va_list args;
      va_start( args, fmt );
      fprintf( stderr, "Error: " );
      vfprintf( stderr, fmt, args );
      fprintf( stderr, "\n" );
      va_end( args );
}


int main()
{
    ArgTest(3,"3123","aaa","123");
    
    system("pause");
    return 0;
}

