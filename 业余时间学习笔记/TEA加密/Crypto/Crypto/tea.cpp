#include "stdafx.h"
#include "tea.h"

void  cTea::Encode( unsigned char *bytes , const unsigned int *key)
{
	unsigned int y;
	unsigned int z;
	unsigned int sum;
	unsigned int n;

	y = *( (unsigned int*)bytes );
	z = *( (unsigned int*)bytes );
	
	register unsigned int delta = 0x9E3779B9;

	sum = 0 ;
	n   = 32;
	
	while( n-- > 0 )
	{
			sum += delta;

			y += ( z << 4 ) + key[0] ^ z + sum ^ ( z >> 5 ) + key[1];
			z += ( y << 4 ) + key[2] ^ y + sum ^ ( y >> 5 ) + key[3];
	}

	*((unsigned int*) bytes ) = y;
	*((unsigned int*) (bytes + 4) ) = z;

}

void  cTea::Decode( unsigned char *bytes , const unsigned int *key)
{
	unsigned int y;
	unsigned int z;
	unsigned int sum;
	unsigned int n;

	y = *((unsigned int*) bytes);
	z = *((unsigned int*) (bytes + 4));
	
	register unsigned int delta = 0x9E3779B9 ;

	sum = delta << 5;
	n = 32;

	while (n-- > 0)
	{
		z -= (y << 4) + key[2] ^ y + sum ^ (y >> 5) + key[3];
		y -= (z << 4) + key[0] ^ z + sum ^ (z >> 5) + key[1];

		sum -= delta;
	}

	*((unsigned int*) bytes) = y;
	*((unsigned int*) (bytes + 4)) = z;
}
