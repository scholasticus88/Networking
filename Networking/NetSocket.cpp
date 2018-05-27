#include "NetSocket.h"

CNetSocket::CNetSocket():
	m_hSocket(INVALID_SOCKET)
{
	memset(&m_addrTarget, 0, sizeof(m_addrTarget));
}

CNetSocket::CNetSocket(SOCKET socket, const SOCKADDR_IN& addr)
{
	this->m_hSocket = socket;
	memcpy(&m_addrTarget, &addr, sizeof(m_addrTarget));
}

bool CNetSocket::Open(const char* pszAddress, PORT usPort)
{
	char pszAddressBuffer[IP_ADDRESS_BUFFER_LEN];
	if (CNetUtils::IsValidIP(pszAddress))
	{
		strncpy(pszAddressBuffer, pszAddress, IP_ADDRESS_BUFFER_LEN);
	}
	else
	{
		CHostInfo hif;
		if (hif.ObtainDataByName(pszAddress))
		{
			hif.GetAdressByIndex(0, pszAddressBuffer);
		} else {
			return false;
		}
	}

	if (!CreateSocket(pszAddress, usPort))
		return false;

	m_addrTarget.sin_family      = AF_INET;
	m_addrTarget.sin_port        = htons(usPort);
	m_addrTarget.sin_addr.s_addr = inet_addr(pszAddressBuffer);

	if (connect(m_hSocket, (SOCKADDR*)&m_addrTarget, sizeof(m_addrTarget)) == SOCKET_ERROR)
	{
		this->Close();
		return false;
	}

	return true;
}

bool CNetSocket::CreateSocket(const char* pszAddress, PORT usPort)
{
	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_hSocket == INVALID_SOCKET)	
		return false;

	return true;
}

bool CNetSocket::Send(CNetPacket* pData)
{
	if (m_hSocket == INVALID_SOCKET)
		return false;

	send(m_hSocket, pData->GetRawData(), pData->GetRawDataLen(), 0);

	return true;
}

bool CNetSocket::Receive(char ** data, long* length)
{
	char buff[1024];
	int l = recv(m_hSocket, buff, 1024, 0);

	if (l <= 3)
		return false;

	*data = new char[l];
	memcpy(*data, buff, l);
	*length = l;

	return true;
}

bool CNetSocket::Close()
{
	if (m_hSocket == INVALID_SOCKET)
	{
		return false;
	}

	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
	return true;
}

bool CNetSocket::GetIPAsString(char * buffer, int len) const
{
	if (buffer == NULL)
		return false;

	strncpy(buffer, inet_ntoa(m_addrTarget.sin_addr), len);
	return true;
}

USHORT CNetSocket::GetPort() const
{
	return ntohs(m_addrTarget.sin_port);
}

bool CNetSocket::IsClosed() const
{
	return (m_hSocket == INVALID_SOCKET);
}

CNetSocket::~CNetSocket(void)
{
	if (m_hSocket != INVALID_SOCKET)
		this->Close();
}
