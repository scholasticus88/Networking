#ifndef __FILELOGGER__

#define __FILELOGGER__

#include "CommonIncludes.h"
#include <stdlib.h>
#include <stdio.h>

class CFileLogger
{
public:
	CFileLogger(const char* pszFileName);
	~CFileLogger(void);

	void LogString(const char* pszModuleName, const char* pszText);

private:
	void CreateLogFile(const char* pszFileName);

	FILE*            m_pFile;
	CRITICAL_SECTION m_hLock;

};

#endif