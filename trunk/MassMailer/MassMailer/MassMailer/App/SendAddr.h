

#pragma once 

#include "../Public/Tools.h"

class  CSendAddr
{
	typedef  std::vector<tagSendAddr>  VecList;

private:
	VecList   m_Vec;

	void	  Load();

public:

	CSendAddr()
	{
		Load();
	}

	tagSendAddr &  GetSendAddr()
	{
		size_t  size = m_Vec.size();

		int     s   = rand()%size;

		return  m_Vec[ s ];
	}

};