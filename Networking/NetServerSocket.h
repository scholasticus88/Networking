#pragma once

#include "CommonIncludes.h"
#include "NetPacket.h"
#include "NetSocket.h"
#include "NetClientServiceRoutine.h"
#include "FileLogger.h"

typedef DWORD (*CLIENT_SERVICE_ROUTINE)(void* pObj, CNetSocket* clientSocket);

class CNetServerSocket : public CNetSocket
{
protected:
	HANDLE m_hServerThread;
	HANDLE m_hCleaningThread;
	HANDLE m_hStopEvent;
	void*  m_pCallbackObj;
	CLIENT_SERVICE_ROUTINE m_pCallbackRoutine;
	CFileLogger* m_pLogger;

	static DWORD __stdcall _CleaningThread(LPVOID);
	static DWORD __stdcall _ServerThread(LPVOID);
	static DWORD __stdcall _StartClientServiceThread(LPVOID);
	DWORD ServerThread();
	DWORD CleaningThread();
	void CleanConnections();

	CNetSocket* Accept();

	struct callback_info
	{
		CNetSocket* clientSocket;
		void*       pCallbackObj;
		CLIENT_SERVICE_ROUTINE pCallbackRoutine;
	};

	struct active_connection
	{
		CNetSocket* pClientSocket;
		HANDLE      hThread;
	};

	std::map<DWORD, active_connection*> m_ThreadMap;
	CRITICAL_SECTION                    m_CriticalSection;

public:
	CNetServerSocket(CFileLogger* pLogger);
	virtual ~CNetServerSocket(void);

	virtual bool Open(const char* pszAddress, PORT usPort);
	virtual bool Run(); 
	virtual bool Stop();
	virtual void SetClientServiceCallback(void* pCallbackObj, CLIENT_SERVICE_ROUTINE pCallbackRoutine);
};
