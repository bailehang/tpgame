#include <iostream>
#include <limits.h>
#include <assert.h>
using namespace std;


#define Min(TYPE)  (std::numeric_limits<TYPE>::min)()
#define Max(TYPE)  (std::numeric_limits<TYPE>::max)()
#define UMin(TYPE) (Min(TYPE)>=0 ? 0 : Min(TYPE) )

#define AssertCheck(Count,TYPE) \
	assert( n >= UMin(TYPE) && n <= Max(TYPE) )

int main()
{
	
	/// int type check
	AssertCheck( -10 , int );
	AssertCheck( -10 , unsigned int);

	/// char
	AssertCheck( -5  , char );
	AssertCheck( -5  , unsigned char);

	return 0;
}
