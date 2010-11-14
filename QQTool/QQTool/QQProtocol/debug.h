#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>
#include <errno.h>
#include <assert.h>

//#define RELEASE

#if defined (__GNUC__)
#  define __FUNC__     ((const char*) (__PRETTY_FUNCTION__))
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 19901L
#  define __FUNC__     ((const char*) (__func__))
#else
#  define __FUNC__     ((const char*) (__FUNCTION__))
#endif


#ifndef RELEASE
#define DBG(args, ...) \
	print_error( __FILE__, (char*)__FUNC__, __LINE__, ##args )
#else
#define DBG(args ...) 
//#define DBG printf
#endif
#define MSG	printf
void print_error(char* file, char* function, int line, const char *fmt, ...);
void hex_dump( unsigned char * buf, int len );
void debug_term_on();
void debug_term_off();
void debug_file_on();
void debug_file_off();
void debug_set_dir(char* str);

#endif //_DEBUG_H

