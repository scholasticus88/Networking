#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "Server.h"
#include "ClientSocket.h"

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    CServer* pServer = new CServer();

    if (!pServer->Run())
    {
        printf("Could not run the server...\n");
        return 0;
    }

    system("PAUSE");

    pServer->Stop();

    delete pServer;
    pServer = NULL;

    system("PAUSE");
    WSACleanup();
	return 0;
}

