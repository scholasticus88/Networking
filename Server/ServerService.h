#ifndef __SERVER_SERVICE__

#define __SERVER_SERVICE__

#include "CommonIncludes.h"
#include "NetClientServiceRoutine.h"
#include "NetPacketCustomer.h"

class CServerService : public CNetClientServiceRoutine
{
public:
	CServerService(void);
	virtual ~CServerService(void);

	virtual DWORD ServiceClientImpl(CNetSocket* clientSocket);
};

#endif