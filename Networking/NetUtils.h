#ifndef __NET_UTILS__

#define __NET_UTILS__

#include "CommonIncludes.h"
#include "HostInfo.h"

class CNetUtils
{
private:
	CNetUtils(void);
	~CNetUtils(void);

public:
	static bool IsValidIP(const char* pszIP);	
	static bool InitSockets();
	static bool UninitSockets();

};

#endif