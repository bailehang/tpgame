
#pragma once

typedef unsigned long  ulong;

/// define m
#define lnum		5
#define sbits		6
#define ebits		8
#define sbitsize	( 1 << sbits )
#define ebitsize	( 1 << ebits )
#define sMask		( sbitsize- 1)
#define eMask		( ebitsize -1)

/// Êý×é
#define SAFE_ARR_DELETE( a )   if((a)!= NULL) {  delete [] a; a = NULL;} 
/// Ö¸Õë
#define SAFE_DELETE( a )	   if((a)!= NULL) {  delete  a ; a = NULL; }


