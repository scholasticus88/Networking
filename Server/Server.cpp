#include "stdafx.h"
#include "CommonIncludes.h"
#include "NetPacketCustomer.h"
#include "NetServerSocket.h"
#include "ServerService.h"

int main(int argc, char* argv[])
{
	CNetUtils::InitSockets();

	CNetServerSocket* p = new CNetServerSocket(new CFileLogger("D:\\MyLog.txt"));
	if (!p->Open("127.0.0.1", 80))
		cout << "Could not start server" << endl;

	CServerService* ss = new CServerService();
	p->SetClientServiceCallback(ss, CServerService::_ServiceClient);

	p->Run();

	cout << "Server is running. Press any key to stop it..." << endl;
	getchar();

	if (!p->Stop())
		cout << "Could not stop server" << endl;

	delete p;
	delete ss;

	CNetUtils::UninitSockets();
	return 0;
}

