#ifndef __NET_SOCKET__

#define __NET_SOCKET__

#include "CommonIncludes.h"
#include "NetUtils.h"
#include "NetPacket.h"

class CNetSocket
{
public:
	CNetSocket();
	CNetSocket(SOCKET socket, const SOCKADDR_IN& addr);
	virtual ~CNetSocket(void);

	virtual bool Open(const char* pszAddress, PORT usPort);
	virtual bool Send(CNetPacket* pData);
	virtual bool Receive(char ** data, long* length);
	virtual bool Close();
	virtual bool IsClosed() const;

	bool   GetIPAsString(char * buffer, int len) const;
	USHORT GetPort() const;

protected:
	SOCKADDR_IN m_addrTarget;
	SOCKET	    m_hSocket;

	bool CreateSocket(const char* pszAddress, PORT usPort);
};

#endif