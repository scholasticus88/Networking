#ifndef __NET_HOSTINFO__

#define __NET_HOSTINFO__

#include "CommonIncludes.h"

class CHostInfo
{
public:
	CHostInfo(void);
	CHostInfo(const struct hostent* pHost);
	~CHostInfo(void);

	void GetHostName(char* name) const;
	void GetAliasByIndex(int index, char* pszAlias) const;
	void GetAdressByIndex(int index, char* pszAdress) const;

	int GetAliasCount() const;
	int GetAddressCount() const;

	bool ObtainDataByName(const char* pszName);
	bool ObtainDataByIP(const char* pszIP);

	void Clear();

private:
	void SetHostName(const char* name);
	void AddAlias(const char* name);
	void AddAddress(const char* name);
	void SetUpFromHostent(const struct hostent* pHost);

	char*		   m_pszHostName;
	vector<char*>  m_vAliases;
	vector<char*>  m_vAdresses;

};

#endif
