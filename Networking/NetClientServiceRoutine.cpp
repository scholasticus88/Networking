#include "NetClientServiceRoutine.h"

CNetClientServiceRoutine::CNetClientServiceRoutine(void)
{
}

CNetClientServiceRoutine::~CNetClientServiceRoutine(void)
{
}

DWORD CNetClientServiceRoutine::ServiceClient(CNetSocket* clientSocket)
{
	DWORD ret = this->ServiceClientImpl(clientSocket);

	clientSocket->Close();
	return ret;
}

DWORD CNetClientServiceRoutine::_ServiceClient(void* pObj, CNetSocket* clientSocket)
{
	CNetClientServiceRoutine* pThis = (CNetClientServiceRoutine*)pObj;
	return pThis->ServiceClient(clientSocket);
}