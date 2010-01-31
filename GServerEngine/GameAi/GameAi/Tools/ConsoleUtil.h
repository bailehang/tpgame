/**
 * File  :
 * Brief :  about  Console Config
 * Author:  Expter  
 * Date  :  [2010-1-23]
 *                             
 **/
#pragma once

inline void SetConsoleTextColor( DWORD Color)
{
	HANDLE	hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	::SetConsoleTextAttribute( hConsole , Color );
}

inline void PressAnyKeyToContinue()
{
	std:cout << " please Press Any key to Continue " << std::endl;
	getchar();
}