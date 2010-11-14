/**
 *  @file	 tools.h	
 *  @author  expter
 *  @brief   some function
 */ 
#pragma  once

template < typename type >
inline  type DereferencetoType( void * val)
{
	return *( type )( val ) ;
}