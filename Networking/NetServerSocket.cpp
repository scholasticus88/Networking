#ifndef __NET_SERVER_SOCKET__

#define __NET_SERVER_SOCKET__

#include "NetServerSocket.h"
#include "NetUtils.h"

CNetServerSocket::CNetServerSocket(CFileLogger* pLogger):
	m_hServerThread(INVALID_HANDLE_VALUE),
	m_hCleaningThread(INVALID_HANDLE_VALUE),
	m_hStopEvent(INVALID_HANDLE_VALUE),
	m_pCallbackObj(NULL),
	m_pCallbackRoutine(NULL),
	m_pLogger(pLogger)
{
	InitializeCriticalSection(&m_CriticalSection);
}

CNetServerSocket::~CNetServerSocket(void)
{
	if ((m_hServerThread != INVALID_HANDLE_VALUE) || (m_hCleaningThread != INVALID_HANDLE_VALUE))
		Stop();
	DeleteCriticalSection(&m_CriticalSection);
}

bool CNetServerSocket::Open(const char* pszAddress, PORT usPort)
{
	if (m_pLogger)
		m_pLogger->LogString("CNetServerSocket", "Calling Open()");

	if (!CNetUtils::IsValidIP(pszAddress))
		return false;

	if (!CreateSocket(pszAddress, usPort))
		return false;
	
	m_addrTarget.sin_family      = AF_INET;
	m_addrTarget.sin_port        = htons(usPort);
	m_addrTarget.sin_addr.s_addr = INADDR_ANY;

	if (bind(m_hSocket, (SOCKADDR*)&m_addrTarget, sizeof(m_addrTarget)) == SOCKET_ERROR)
		return false;

	if (listen(m_hSocket, 10) == SOCKET_ERROR)
		return false;

	return true;
}

bool CNetServerSocket::Run()
{
	if (m_pLogger)
		m_pLogger->LogString("CNetServerSocket", "Calling Run()"); 

	if ((m_hServerThread != INVALID_HANDLE_VALUE) || (m_hCleaningThread != INVALID_HANDLE_VALUE))
		return false;

	m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hStopEvent == INVALID_HANDLE_VALUE)
		return false;

	m_hCleaningThread = CreateThread(NULL, 0, _CleaningThread, this, NULL, NULL);
	if (m_hCleaningThread == INVALID_HANDLE_VALUE)
		return false;

	m_hServerThread = CreateThread(NULL, 0, _ServerThread, this, NULL, NULL);
	if (m_hServerThread == INVALID_HANDLE_VALUE)
		return false;
	
	return true;
}

bool CNetServerSocket::Stop()
{
	if (m_pLogger)
		m_pLogger->LogString("CNetServerSocket", "Calling Stop() - start"); 

	if ((m_hServerThread == INVALID_HANDLE_VALUE) && (m_hCleaningThread == INVALID_HANDLE_VALUE))
		return false;

	SetEvent(m_hStopEvent);

	if (m_pLogger)
		m_pLogger->LogString("CNetServerSocket", "Calling Stop() - waiting for ServerThread to terminate."); 

	if (m_hServerThread != INVALID_HANDLE_VALUE)
	{
		if (WaitForSingleObject(m_hServerThread, 5000) == WAIT_TIMEOUT)
			TerminateThread(m_hServerThread, 0);
		m_hServerThread = INVALID_HANDLE_VALUE;
	}

	if (m_pLogger)
		m_pLogger->LogString("CNetServerSocket", "Calling Stop() - waiting for CleaningThread to terminate."); 

	if (m_hCleaningThread != INVALID_HANDLE_VALUE)
	{
		if (WaitForSingleObject(m_hCleaningThread, 5000) == WAIT_TIMEOUT)
			TerminateThread(m_hCleaningThread, 0);
		m_hCleaningThread = INVALID_HANDLE_VALUE;
	}

	if (m_pLogger)
		m_pLogger->LogString("CNetServerSocket", "Calling Stop() - finished."); 

	return true;
}

DWORD __stdcall CNetServerSocket::_CleaningThread(LPVOID pParam)
{
	CNetServerSocket* pThis = (CNetServerSocket*)pParam;
	return pThis->CleaningThread();
}

DWORD __stdcall CNetServerSocket::_ServerThread(LPVOID pParam)
{
	CNetServerSocket* pThis = (CNetServerSocket*)pParam;
	return pThis->ServerThread();
}

DWORD CNetServerSocket::CleaningThread()
{
	bool bStop  = false;
	
	while (!bStop)
	{
		int iVal = WaitForSingleObject(m_hStopEvent, 2500);
		switch (iVal)
		{
			case WAIT_OBJECT_0:
				bStop = true;
				break;

			case WAIT_TIMEOUT:
				CleanConnections();
				break;
		}
	}

	return 0;
}

void CNetServerSocket::CleanConnections()
{
	EnterCriticalSection(&m_CriticalSection);

	std::vector<DWORD> idToDelete;

	std::map<DWORD, active_connection*>::iterator it = m_ThreadMap.begin();
	for (; it != m_ThreadMap.end(); it++)
	{
		struct active_connection* ac = it->second;
		if (ac != NULL)
		{
			if ((ac->pClientSocket != NULL) && ac->pClientSocket->IsClosed())
			{
				if (m_pLogger)
				{
					char txt[512];
					char ip[30];
					ac->pClientSocket->GetIPAsString(ip, 30);
					sprintf(txt, "Clearing closed connection [ThreadId=%d, Addr=%s:%d]", it->first, ip, ac->pClientSocket->GetPort());
					m_pLogger->LogString("CNetServerSocket", txt); 
				}

				CloseHandle(ac->hThread);
				delete ac->pClientSocket;
				ac->pClientSocket = NULL;
				idToDelete.insert(idToDelete.end(), it->first);
			}
		}
	}

	for (std::vector<DWORD>::iterator it2 = idToDelete.begin(); it2 != idToDelete.end(); it2++)
	{
		std::map<DWORD, active_connection*>::iterator found = m_ThreadMap.find(*it2);
		if (found != m_ThreadMap.end())
		{
			delete found->second;
			m_ThreadMap.erase(found);
		}
	}
	idToDelete.clear();

	LeaveCriticalSection(&m_CriticalSection);	
}

DWORD CNetServerSocket::ServerThread()
{
	bool bStop  = false;

	if (m_pLogger)
		m_pLogger->LogString("CNetServerSocket", "Server is running...");

	while (!bStop)
	{
		CNetSocket* clienSocket = Accept();
		if (clienSocket != NULL)
		{
			if ((m_pCallbackObj != NULL) && (m_pCallbackRoutine != NULL))
			{
				struct callback_info* cbi = new struct callback_info;
				cbi->clientSocket     = clienSocket;
				cbi->pCallbackObj     = m_pCallbackObj;
				cbi->pCallbackRoutine = m_pCallbackRoutine;

				DWORD  dwThreadId   = 0;
				HANDLE hClienthread = CreateThread(NULL, 0, _StartClientServiceThread, cbi, 0, &dwThreadId);

				EnterCriticalSection(&m_CriticalSection);
				struct active_connection* ac = new struct active_connection;
				ac->hThread                  = hClienthread;
				ac->pClientSocket            = clienSocket;
				m_ThreadMap[dwThreadId]      = ac;

				if (m_pLogger)
				{
					char txt[512];
					char ip[30];
					clienSocket->GetIPAsString(ip, 30);
					sprintf(txt, "New client was accepted [%s:%d]", ip, clienSocket->GetPort());
					m_pLogger->LogString("CNetServerSocket", txt);
				}

				LeaveCriticalSection(&m_CriticalSection);

			} else {
				clienSocket->Close();
				delete clienSocket;
			}
		}
	}

	return 0;
}

CNetSocket* CNetServerSocket::Accept()
{
	CNetSocket* clientSocket = NULL;
	sockaddr_in addr;

	int  len = sizeof(addr);
	SOCKET s = accept(m_hSocket, (sockaddr*)&addr, &len);
	if (s != INVALID_SOCKET)
	{
		clientSocket = new CNetSocket(s, addr);
	}

	return clientSocket;
}

void CNetServerSocket::SetClientServiceCallback(void* pCallbackObj, CLIENT_SERVICE_ROUTINE pCallbackRoutine)
{
	m_pCallbackObj     = pCallbackObj;
	m_pCallbackRoutine = pCallbackRoutine;
}

DWORD CNetServerSocket::_StartClientServiceThread(LPVOID pParam)
{
	struct callback_info* pcb = (struct callback_info*)pParam;

	CLIENT_SERVICE_ROUTINE pRtn = pcb->pCallbackRoutine;
	CNetSocket* pSocket         = pcb->clientSocket;
	void*       pObj            = pcb->pCallbackObj;
	delete pcb;

	return pRtn(pObj, pSocket);
}

#endif