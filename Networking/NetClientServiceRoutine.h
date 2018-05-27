#ifndef __NET_CLIENT_SERVICE__

#define __NET_CLIENT_SERVICE__

#include "CommonIncludes.h"
#include "NetSocket.h"

class CNetClientServiceRoutine
{
public:
	CNetClientServiceRoutine(void);
	virtual ~CNetClientServiceRoutine(void);

	static DWORD _ServiceClient(void* pObj, CNetSocket* clientSocket);

protected:
	virtual DWORD ServiceClientImpl(CNetSocket* clientSocket) = 0;
	DWORD ServiceClient(CNetSocket* clientSocket);
};

#endif