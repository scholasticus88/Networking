#include "FileLogger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

CFileLogger::CFileLogger(const char* pszFileName)
{
	CreateLogFile(pszFileName);
	InitializeCriticalSection(&m_hLock);
}

CFileLogger::~CFileLogger(void)
{
	if (m_pFile)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

	DeleteCriticalSection(&m_hLock);
}

void CFileLogger::CreateLogFile(const char* pszFileName)
{
	m_pFile = fopen(pszFileName, "w+");
	if (m_pFile == NULL)
		throw "Could not create logfile!";
}

void CFileLogger::LogString(const char* pszModuleName, const char* pszText)
{
	char pszTextLine[1024] = {0};
	char pszTime[128]      = {0};

	if (m_pFile)
	{
		EnterCriticalSection(&m_hLock);
	
		SYSTEMTIME st;
		GetLocalTime(&st);

		sprintf(pszTime, "%02d.%02d.%d-%02d:%02d:%02d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond); 

		sprintf(pszTextLine, "%20s\t%20s:\t%s\n", pszTime, pszModuleName, pszText);

		fwrite(pszTextLine, 1, strlen(pszTextLine), m_pFile);
		fflush(m_pFile);

		LeaveCriticalSection(&m_hLock);
	}
}

