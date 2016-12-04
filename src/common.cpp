#include "common.h"

void Msg(HWND hwnd,TCHAR *szFormat, ...)
{
	TCHAR szBuffer[1024];  
	const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
	const int LASTCHAR = NUMCHARS - 1;

	va_list pArgs;
	va_start(pArgs, szFormat);

	(void)StringCchVPrintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
	va_end(pArgs);

	szBuffer[LASTCHAR] = TEXT('\0');

	MessageBox(hwnd, szBuffer, TEXT("Message"), MB_OK | MB_ICONERROR);
}

bool Bstr_Compare(BSTR bstrFilter,BSTR bstrDevice)
{
	bool flag = true;
	int strlenFilter = SysStringLen(bstrFilter);
	int strlenDevice = SysStringLen(bstrDevice);
	char* chrFilter = (char*)malloc(strlenFilter+1);
	char* chrDevice = (char*)malloc(strlenDevice+1);
	int j = 0;

	if (strlenFilter!=strlenDevice)
		flag = false;
	else
	{
		for(; j < strlenFilter;j++)
		{
			chrFilter[j] = (char)bstrFilter[j];
			chrDevice[j] = (char)bstrDevice[j];

		}
		chrFilter[strlenFilter] = '\0';
		chrDevice[strlenDevice] = '\0';

		for(j=0; j < strlenFilter;j++)
		{
			if(chrFilter[j] != chrDevice[j])
				flag = false;
		}

		if(flag == true && j == strlenFilter-1)
			flag = true;
	}
	return flag;
}