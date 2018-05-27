#ifndef __SERVER_SOCKET__
#define __SERVER_SOCKET__

#include "ClientSocket.h"

class CServerSocket
{
    public:
        CServerSocket();
        ~CServerSocket();

        bool Open(char* pIPAddress, short sPort);
        CClientSocket* TryAccept(int iTimeout);
        bool Close();

    private:
        SOCKET m_hSocket;
};

#endif