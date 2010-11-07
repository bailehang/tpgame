

#pragma once 

#include <string>
#include <cstdlib>

using namespace std;


long  Random(long Num)
{
	return  rand() % Num;
}

void  StrReplace( std::string str, std::string& sou, std::string& des)
{
	char  szText [256],szDay[256];

	time_t timep;

	struct tm *p;

	time(&timep);

	p=gmtime(&timep);

	sprintf( szDay, "%d-%d-%d" , p->tm_year+1990,p->tm_mon+1,p->tm_mday);
	sprintf( szText,"%d:%d:%d" , p->tm_hour,p->tm_min,p->tm_sec);

	if (des == "#time")
	{
		str.replace(sou,szText);
	}
	else if (des == "#day")
	{
		str.replace(sou,szDay);
	}
	else if (des == "#name")
	{

	}
	else 
	{
		str.replace(sou,des);
	}
}

/**
 *   @brief
 *      str是需要替换的字符串
 *		max是包含@的个数
 *		sNum是可替换的数量
 *		Rand不替换字符串的概率
 */
void   ttos( std::string& str , long max,long sNum,long Rand,long Rand1,long Rand2,long Rand3)
{
	long  lRand =   Random( 100 );
	
	/// 
	if ( lRand <= Rand )
	{
		for ( int i = 0 ; i < max ; i++ )
		{
			StrReplace(str,"@","");
		}
	}
	/// 替换一个字符
	else if ( lRand < Rand1 )
	{
		long  mNum  =   Random( sNum);
		long  iNum  =   Random( max );

		for ( int i = 0 ; i < max ; i++ )
		{
			if( iNum != i  )
				StrReplace(str,"@","");//str.replace("@","");
			else
				StrReplace(str,"@","\\");
		}
	}
	else if ( lRand < Rand2 )
	{
		long  mNum   =   Random( sNum);

		long  iNum1,iNum2;

		long  iNum1  =   Random( max );

		while( 1 )
		{
			iNum2  =   Random( max );
			if ( iNum2 != iNum1 )
			{
				break;
			}
		}
		for ( int i = 0 ; i < max ; i++ )
		{
			if( iNum1 == i  )
				StrReplace(str,"@","1");
			else if( iNum2 = i )
				StrReplace(str,"@","2");
			else 
				StrReplace(str,"@","");
		}
	}
	else if ( lRand < Rand3 )
	{
		long  iNum1,iNum2,iNum3;

		long  iNum1  =   Random( max );

		while( 1 )
		{
			iNum2  =   Random( max );
			if ( iNum2 != iNum1 )
			{
				break;
			}
		}
		while( 1 )
		{
			iNum3  =   Random( max );
			if ( iNum3 != iNum1  && iNum3 != iNum2)
			{
				break;
			}
		}
		for ( int i = 0 ; i < max ; i++ )
		{
			if( iNum1 == i  )
				StrReplace(str,"@","1");
			else if( iNum2 = i )
				StrReplace(str,"@","2");
			else if( iNum3 = i )
				StrReplace(str,"@","3");
			else 
				StrReplace(str,"@","");
		}
	}

}
