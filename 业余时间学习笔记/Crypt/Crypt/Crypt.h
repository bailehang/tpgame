#pragma once 


const int c1 = 52845;
const int c2 = 22719;
const int key= 65372;

typedef unsigned char BYTE;
class  CCrypt
{
public:

	/// º”√‹
	static  bool  EnCrypt(BYTE* source ,  BYTE*  destion, size_t len );

	/// Ω‚√‹
	static  bool  DeCrypt(BYTE* source ,  BYTE*  destion, size_t len );

};


bool  CCrypt::EnCrypt(BYTE* source , BYTE* destion, size_t len )
{
	int   keyw = key;
	if ( !source || !destion || len <= 0 )
		return false;

	for ( size_t size = 0 ; size < len ; size ++ )
	{
		destion[size] = source[size] ^ keyw >> 8;
		keyw  = ( destion[size] + keyw ) * c1 + c2;
	}
	return true;
}

bool  CCrypt::DeCrypt(BYTE *source, BYTE *destion, size_t len)
{
	if ( !source || !destion || len <= 0 )
		return false;

	int keyw =key;
	for ( size_t size = 0 ; size < len ; size ++ )
	{
		BYTE  temp = source[size];
		destion[size]  = source[size] ^ keyw >>8;
		keyw = ( temp + keyw ) * c1 + c2;
	}
	return true;
}