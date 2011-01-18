 /**
  *	 @brief 一些公共函数定义和实现  
  */

#pragma  once 


///  二分查找索引
static  inline  long FindIndx(const long *Arr, int l , int r ,long Value)
{
	int low = ( l+r )/2;

	if( Arr[low] == Value ) return Arr[low];
	else if( low == 0 )
	{ 
		if( Arr[low] >= Value)
			return Arr[low];
		return Arr[r];
	}
	else if ( low == r )    return Arr[r];
	else if ( low > 0 && low <= r-2 && Value > Arr[low-1] && Value < Arr[low] ) return Arr[low];
	else if ( Arr[low] < Value ) return FindIndx(Arr,low+1,r,Value); 
	else if ( Arr[low] > Value ) return FindIndx(Arr,l,low-1,Value);
	return Arr[low];
}