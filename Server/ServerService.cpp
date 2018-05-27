#include "StdAfx.h"
#include "ServerService.h"

CServerService::CServerService(void)
{
}

CServerService::~CServerService(void)
{
}

DWORD CServerService::ServiceClientImpl(CNetSocket* clientSocket)
{
	char   ip[20];
	USHORT port;
	clientSocket->GetIPAsString(ip, 20);
	port = clientSocket->GetPort();

	cout << "New connection from " << ip << ":" << port << endl;

	char * data;
	long len;
	if (clientSocket->Receive(&data, &len))
	{
		if (reinterpret_cast<CNetPacket::CNP_MESSAGE*>(data)->msgId == 1)
		{
			CNetPacketCustomer::CNP_CUSTOMER* c = reinterpret_cast<CNetPacketCustomer::CNP_CUSTOMER*>(data);
			cout << "\t ID:       " << c->Id << endl;
			cout << "\t Jmeno:    " << c->FirstName << endl;
			cout << "\t Prijmeni: " << c->LastName << endl;
			cout << "\t Datum:    " << (int)c->Born.day << "." << (int)c->Born.month << "." << c->Born.year << endl;
		}
		delete data;
	}

	return 0;
}
