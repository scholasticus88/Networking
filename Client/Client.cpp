#include "stdafx.h"
#include "Networking.h"
#include "NetPacketCustomer.h"

int main(int argc, char* argv[])
{
	if (!CNetUtils::InitSockets())
	{
		cout << "Could not initialize sockets." << endl;
		return 0;
	}

	CNetSocket* ns = new CNetSocket();
	if (ns->Open("localhost", 80))
	{
		CNetPacketCustomer* packet = new CNetPacketCustomer();
		packet->CreateBuffer();
		packet->SetId(1);
		packet->SetFirstName("Jan");
		packet->SetLastName("Kucera");
		packet->SetBorn(25, 2, 1988);

		if (!ns->Send(packet))
		{
			cout << "Sending data failed..." << endl;
		}

		delete packet;
		packet = NULL;

		ns->Close();
	} else cout << "Could not open connection..." << endl;

	delete ns;

	CNetUtils::UninitSockets();
	
	system("PAUSE");
	return 0;
}

