/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2009/11/2]
*/

#pragma  once

class  CBitTable
{

public:

	template < class T >
	void Init( T * table)
	{
		ZeroMemory( table , sizeof( T) );
	}

	template < class T >
	T  ChangeOnBit( T & table , BYTE Pos )
	{
		T temp = 1;
		temp <<= Pos;

		return ( table | temp );
	}

	template < class T >
	T ChangeOffBit(T& table,BYTE Pos)
	{
		T Temp = 1;
		Temp <<= Pos;

		return (table & (~Temp));
	}

	template < class T >
	T QesOnBit(T& table,BYTE Pos)
	{
		T Temp = 1;
		Temp <<= Pos;

		return (table & Temp);
	}

};