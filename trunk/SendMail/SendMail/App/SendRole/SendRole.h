
#pragma  once 

#include <string>

long    Random(long Value)
{						   
	return  rand() % Value ;
}

/// str是sub , Size是@个数
void	ttos(std::string  str,long Size,long Rand,long Rand1,long Rand2,long Rand3)
{
	long  tRand = Random( Size );
	long  sChange = Random( 100 );

	if ( sChange < Rand )
	{
		str.replace("@","");
	}
	else if ( )

	//for ( int i = 0 ; i < )
	{
	}
		
}