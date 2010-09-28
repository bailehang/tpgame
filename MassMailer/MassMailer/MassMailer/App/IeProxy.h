// 
// 
// #pragma  once
// 
// 
// bool   IeProxy()
// {
// 	HINTERNET m_hInternetSession= ::InternetOpen (NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0); 
// 	if (!m_hInternetSession) 
// 	{ 
// 		TRACE(_T("InternetOpen () failed\n")); 
// 	} 
// 
// 	typedef struct { 
// 		DWORD dwAccessType; 
// 		LPCSTR lpszProxy; 
// 		LPCSTR lpszProxyBypass; 
// 	} INTERNET_PROXY_INFOA, * LPINTERNET_PROXY_INFOA; 
// 
// 	DWORD dwRet = 0; 
// 	INTERNET_PROXY_INFOA ipi; 
// 	DWORD dwSize = sizeof(ipi); 
// 
// 	ipi.dwAccessType = INTERNET_OPEN_TYPE_PROXY ; 
// 	ipi.lpszProxy = "代理IP地址"; 
// 	ipi.lpszProxyBypass = NULL; 
// 	::InternetSetOption(m_hInternetSession, INTERNET_OPTION_PROXY, &ipi, dwSize); 
// 
// 	HINTERNET hHttpConnection = ::InternetConnect(m_hInternetSession, "要访问的地址", 
// 		 
// 		, NULL, NULL, INTERNET_SERVICE_HTTP , dwFlag, 0); 
// 	if(!hHttpConnection) 
// 	{ 
// 		TRACE("InternetConnect: %d\n", GetLastError()); 
// 		return ; 
// 	} 
// 	HINTERNET hHttpRequest = ::HttpOpenRequest(hHttpConnection, _T("GET"), 
// 		"网址", NULL, NULL, NULL, dwFlag, 0); 
// 
// 	if(!::HttpSendRequest(hHttpRequest,NULL, 0, NULL,0)) 
// 	{ 
// 		::InternetCloseHandle(hHttpRequest); 
// 		::InternetCloseHandle(hHttpConnection); 
// 		return dwRet; 
// 	} 
// 	TCHAR szBuffer[80]; 
// 	DWORD dwLen = sizeof(szBuffer); 
// 
// 	if(::HttpQueryInfo(hHttpRequest, HTTP_QUERY_STATUS_CODE, 
// 		szBuffer, &dwLen, NULL)) 
// 		dwRet = (DWORD) _ttol(szBuffer); 
// 	CString csResponse; 
// 	if(dwRet == 200) 
// 	{ 
// 		BYTE pBuffer[HTTPGET_BUFFER_MAX+1]; 
// 
// 		DWORD dwRead; 
// 		while(::InternetReadFile(hHttpRequest, pBuffer, HTTPGET_BUFFER_MAX, 
// 			&dwRead)) 
// 		{ 
// 			if(dwRead == 0) 
// 				break; 
// 
// 			pBuffer[dwRead] = 0; 
// 			csResponse += (char*)pBuffer; 
// 		} 
// 	} 
// 
// 
// }
// 
