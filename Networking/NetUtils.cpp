#include "NetUtils.h"

CNetUtils::CNetUtils(void)
{
}

CNetUtils::~CNetUtils(void)
{
}

bool CNetUtils::IsValidIP(const char* pszIP)
{
	bool bRetVal = true;
	char buff[5] = {0};
	int  i       = 0;
	int iPart    = 0;
	int iVal     = 0;
	int len      = 0;

	if (pszIP == NULL)
		return false;

	len = strlen(pszIP);
	
	while (iPart < 4)
	{
		iVal = 0;
		while (IS_DIGIT(pszIP[i]))
		{
			iVal = iVal*10 + (pszIP[i] - '0');
			i++;
		}
		if (!((iVal >= 0) && (iVal <= 255)))
			return false;
		if ((iPart < 3) && ((pszIP[i] != '.') || !(i < len) || !IS_DIGIT(pszIP[i+1])))
			return false;		
		if ((iPart == 3) && (pszIP[i] != 0))
			return false;
		i++;
		iPart++;
	}

	return bRetVal;
}

bool CNetUtils::InitSockets()
{
	WSAData wsad;
	int     err;
	
	err = WSAStartup(MAKEWORD(2, 2), &wsad);
	if (err)
		return false;

	return true;
}

bool CNetUtils::UninitSockets()
{
	return !(WSACleanup());
}