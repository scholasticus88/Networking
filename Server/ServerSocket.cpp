#include "StdAfx.h"
#include "ServerSocket.h"


CServerSocket::CServerSocket(void):
    m_hSocket(INVALID_SOCKET)
{
}

CServerSocket::~CServerSocket(void)
{
	Close();
}

bool CServerSocket::Open(char* pIPAddress, short sPort)
{
    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_hSocket == INVALID_SOCKET)
    {
        return false;
    }

    u_long on = 1;
    if (ioctlsocket(m_hSocket, FIONBIO, &on) == SOCKET_ERROR)
    {
        return false;
    }

    sockaddr_in addrin;
    addrin.sin_family = AF_INET;
    addrin.sin_addr.S_un.S_addr = inet_addr(pIPAddress);
    addrin.sin_port = htons(sPort);
    if (bind(m_hSocket, (SOCKADDR*)&addrin, sizeof(addrin)) == SOCKET_ERROR)
    {
        return false;
    }

    if (listen(m_hSocket, 128) == SOCKET_ERROR)
    {
        return false;
    }

    return true;
}

CClientSocket* CServerSocket::TryAccept(int iTimeout)
{
    CClientSocket* pClient = NULL;
    fd_set readSet;
    TIMEVAL tv;

    FD_ZERO(&readSet);
    FD_SET(m_hSocket, &readSet);

    tv.tv_sec  = iTimeout;
    tv.tv_usec = 0;

    int iRetVal = select(0, &readSet, NULL, NULL, &tv);
    if (iRetVal > 0)
    {
        if (FD_ISSET(m_hSocket, &readSet))
        {
            SOCKET clientSocket = accept(m_hSocket, NULL, NULL);
            pClient = new CClientSocket(clientSocket);
        }
    }

    return pClient;
}

bool CServerSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}

    return true;
}