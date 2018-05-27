#ifndef __CLIENT_SOCKET__
#define __CLIENT_SOCKET__

#include <string>

class CClientSocket
{
public:
	CClientSocket();
    CClientSocket(SOCKET s);
    ~CClientSocket(void);

    bool         Open(char* pIPAddress, short sPort);
    bool         Write(char* buffer, int iLength);
	bool         IsPreparedToRead();
	bool         IsClosed();
    bool         Close();
    u_short      GetLocalPort();
    u_short      GetRemotePort();
    SOCKET       GetSocket();
	bool         Read(std::string& msg);
	bool         Send(std::string& msg);

private:
    bool        GetPeerName(char* buffer, int iLength);
    bool        GetSockName(char* buffer, int iLength);

    SOCKET m_hSocket;
};

#endif