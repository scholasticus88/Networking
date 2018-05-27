#include "StdAfx.h"
#include "ClientSocket.h"

CClientSocket::CClientSocket():
	m_hSocket(INVALID_SOCKET)
{
}

CClientSocket::CClientSocket(SOCKET s):
	m_hSocket(s)
{
}

bool CClientSocket::Open(char* pIPAddress, short sPort)
{
	if (m_hSocket != INVALID_SOCKET)
		return false;

	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_hSocket == INVALID_SOCKET)
    {
        return false;
    }

	sockaddr_in addrin;
    addrin.sin_family = AF_INET;
    addrin.sin_addr.S_un.S_addr = inet_addr(pIPAddress);
    addrin.sin_port = htons(sPort);
	if (connect(m_hSocket, (SOCKADDR*)&addrin, sizeof(addrin)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool CClientSocket::IsClosed()
{
	return (m_hSocket == INVALID_SOCKET);
}

bool CClientSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = m_hSocket;
	}

	return true;
}

bool CClientSocket::IsPreparedToRead()
{
	bool    bRetVal = false;
	fd_set  readSet;
    TIMEVAL tv;

    FD_ZERO(&readSet);
    FD_SET(m_hSocket, &readSet);

    tv.tv_sec  = 0;
    tv.tv_usec = 100;

    int iRetVal = select(0, &readSet, NULL, NULL, &tv);
    if (iRetVal > 0)
    {
        if (FD_ISSET(m_hSocket, &readSet))
        {
			bRetVal = true;
		}
	}

	return bRetVal;
}

bool CClientSocket::Read(std::string& msg)
{
	char* buff    = NULL;
	bool  bRetVal = false;

	u_long iLength = 0;
	if (ioctlsocket(m_hSocket, FIONREAD, &iLength) != NO_ERROR)
		return false;

	if (iLength > 0)
	{
		buff = new char[iLength+1];

		int iRet = 0;
		if ((iRet = recv(m_hSocket, buff, iLength, 0)) > 0)
		{
			buff[iRet] = NULL;
			msg.append(buff);
			bRetVal = true;
		}
	}

	return bRetVal;
}

bool CClientSocket::Send(std::string& msg)
{
	if (send(m_hSocket, msg.c_str(), msg.length(), 0) < 0)
		return false;
	return true;
}

CClientSocket::~CClientSocket(void)
{
	Close();
}
