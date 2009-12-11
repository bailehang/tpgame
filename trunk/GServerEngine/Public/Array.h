/*
* Brief :  一个自定义支持多位的数组类
* Author:  Expter  
* Date  :  [2009-11-1]
*                             
**/

#pragma  once

class  CArray
{
public:
	
	/*
	 *	一维数组	
	 */
	template < typename type>
	type * AllocArray( type * array , int n)
	{
		array  =  new type[ n ];

		return  array;
	}
	
	template < typename type>
	void ReleaseArray( type * array)										
	{ 
		if(array) delete [] array;
	}

	/*
	*	二维数组	
	*/
	template < typename type>
	type** AllocArray(type ** array, int row, int col)						
	{ 
		type** tmp; 
		tmp = new type*[row]; 

		array = tmp; 

		for(int i=0; i<row ; i++ )  
			tmp[i] = new type[col]; 

		return array;
	}

	template < typename type >
	void ReleaseArray(type ** array, int row, int col)						
	{ 
		if(array)
		{ 
			type **temp1, **temp2; 
			temp1 = temp2 = array; 
			for(int i=0; i < row ; i++)        
			{ 
				temp2 = temp1; 
				temp1++;
				delete [] *temp2; 
			} 
			delete array; 
		}
	}

	/*
	 * 三维数组	
	 */
	template < typename type >
	type***  AllocArray(type *** array, int block, int row, int col)		
	{ 
		type*** tmp; 
		type** tmp2;

		tmp = new type**[block]; 
		array = tmp; 

		for(int i=0; i < block ; i++) 
		{
			tmp[i] = new type*[row]; 
			tmp2 = tmp[i];

			for(int j=0; j < row ; j++) tmp2[j] = new type[col];

		}

		return array;
	}

	template < typename type >
	void ReleaseArray(type *** array, int block, int row, int col)		
	{ 
		if(array)
		{ 
			type ***temp1,***temp2;
			temp1 = temp2 = array;
			for(int i=0; i < block ; i++)      
			{ 
				temp2 = temp1;
				ReleaseArray(*temp2, row, col);
				temp1++;
			} 
			delete [] array;
		}
	}
	
};