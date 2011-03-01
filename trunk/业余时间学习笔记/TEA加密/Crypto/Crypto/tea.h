
#ifndef _TEA_H_
#define _TEA_H_

class  cTea
{
	public:
		/**
		 * @ brief  encode bytes.
		 * @ param  bytes 
		 * @ param  key   
		 */	
		static void  Encode( unsigned char *bytes , const unsigned int *key);

		/**
		 * @ brief  decode bytes 
		 * @ param  bytes
		 * @ param  key
		 */
		static void  Decode( unsigned char *bytes , const unsigned int *key);
};

#endif  /// _END_  _TEA_H
