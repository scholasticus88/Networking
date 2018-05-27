#ifndef __CHAT_SERVER__
#define __CHAT_SERVER__

#include <Windows.h>
#include <vector>
#include <deque>
#include "ServerSocket.h"
#include "ClientSocket.h"

typedef std::vector<CClientSocket*> CLIENTVECTOR;
typedef std::deque<std::string> REQUESTTVECTOR;

class CServer
{
public:
    CServer(void);
    ~CServer(void);
    
    bool Run();
    bool Stop();

private:
    static DWORD __stdcall _ServerRunInternal(LPVOID pParam);
	static DWORD __stdcall _ServerRequestThread(LPVOID pParam);
	static DWORD __stdcall _ServerResponseThread(LPVOID pParam);
    DWORD ServerRunInternal();
	DWORD ServerRequestThread();
	DWORD ServerResponseThread();

	bool  AddClientConnection(CClientSocket* pClient);
	void  AddRequest(std::string& msg);
	void  ProcessRequest(std::string& request);

    HANDLE         m_hListenThread;
	HANDLE         m_hRequestThread;
	HANDLE         m_hResponseThread;
    HANDLE         m_hExitEvent;
	HANDLE         m_hClientVectorMutex;
	HANDLE         m_hRequestVectorMutex;
    
	CServerSocket* pServerSocket;

    CLIENTVECTOR   activeConnections;
	REQUESTTVECTOR requestVector;
};

#endif