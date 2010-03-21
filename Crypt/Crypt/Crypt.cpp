// Crypt.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Crypt.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	char str[20] ="12345";
	char des[20] ={0};
	char sou[20] ={0};
	CCrypt::EnCrypt( (BYTE*)str, (BYTE*)des , strlen(str) );
	std::cout << des << std::endl;
	CCrypt::DeCrypt( (BYTE*)des, (BYTE*)sou , strlen(des) );

	std::cout << sou << std::endl;
	return 0;
}

