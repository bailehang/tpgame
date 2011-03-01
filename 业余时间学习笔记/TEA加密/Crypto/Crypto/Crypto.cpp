// Crypto.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tea.h"
#include <string>
#include <iostream>

using namespace  std;

int _tmain(int argc, _TCHAR* argv[])
{
	cTea tea;

	unsigned int key [ ]  = { 11 , 669 , 773 , 223};
	unsigned char bytes[]  = {"ssssssssssss11111ad111111111dddddddddddddddd"};
	
	tea.Encode( bytes , key );

	std::cout << bytes << std::endl;

	tea.Decode( bytes , key );
	
	std::cout << bytes << std::endl;

	return 0;
}

