
#pragma  once

bool  isRead(ifstream& stream,const char* key)
{
	string str;
	stream >> str;
	
	while( str.c_str() != key) )
	{
		if ( stream.eof() )
		{
			return  false;
		}
		stream >> str;
	}
	return true;
}

bool  isWrite(ofstream& stream,const char* key)
{
	stream >> key;
	return true;
}