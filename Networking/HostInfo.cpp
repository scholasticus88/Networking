#include "HostInfo.h"

CHostInfo::CHostInfo(void):
	m_pszHostName(NULL)
{
	m_vAliases.clear();
	m_vAdresses.clear();
}

CHostInfo::CHostInfo(const struct hostent* data)
{
	this->SetUpFromHostent(data);
}

void CHostInfo::Clear()
{
	if (m_pszHostName != NULL)
	{
		delete m_pszHostName;
		m_pszHostName = NULL;
	}

	for (vector<char*>::iterator it = m_vAdresses.begin(); it != m_vAdresses.end(); it++)
		delete *it;
	m_vAdresses.clear();

	for (vector<char*>::iterator it = m_vAliases.begin(); it != m_vAliases.end(); it++)
		delete *it;
	m_vAliases.clear();
}

void CHostInfo::SetHostName(const char* name)
{
	if (name == NULL)
		return ;

	if (m_pszHostName != NULL)
	{
		delete m_pszHostName;
	}

	int len = strlen(name);
	m_pszHostName = new char[len+1];
	strncpy(m_pszHostName, name, len+1);
}

void CHostInfo::AddAlias(const char* name)
{
	if (name == NULL)
		return ;

	int len  = strlen(name);
	char* nn = new char[len+1];
	strncpy(nn, name, len+1);

	m_vAliases.push_back(nn);
}

void CHostInfo::AddAddress(const char* address)
{
	if (address == NULL)
		return ;

	int len  = strlen(address);
	char* nn = new char[len+1];
	strcpy(nn, address);

	m_vAdresses.push_back(nn);
}

void CHostInfo::GetHostName(char* name) const
{
	if (name == NULL)
		return ;

	if (m_pszHostName != NULL)
		strncpy(name, m_pszHostName, NETWORK_NAME_BUFFER_LEN);
	else name[0] = NULL;
}

void CHostInfo::GetAliasByIndex(int index, char* name) const
{
	if (name == NULL)
		return ;

	if (index < m_vAliases.size())
	{
		strcpy(name, m_vAliases.at(index));
	} else name[0] = NULL;
}

void CHostInfo::GetAdressByIndex(int index, char* name) const
{
	if (name == NULL)
		return ;

	if (index < m_vAdresses.size())
	{
		strcpy(name, m_vAdresses.at(index));
	} else name[0] = NULL;
}

int CHostInfo::GetAliasCount() const
{
	return m_vAliases.size();
}

int CHostInfo::GetAddressCount() const
{
	return m_vAdresses.size();
}

bool CHostInfo::ObtainDataByName(const char* pszName)
{
	this->Clear();

	if (pszName == NULL)
		return false;

	struct hostent* h = gethostbyname(pszName);
	if (h != NULL)
		this->SetUpFromHostent(h);

	return true;
}

bool CHostInfo::ObtainDataByIP(const char* pszIP)
{
	struct sockaddr_in sa;

	this->Clear();

	if (pszIP == NULL)
		return false;

	inet_pton(AF_INET, pszIP, &sa.sin_addr);

	struct hostent* h = gethostbyaddr((char*)&sa.sin_addr, 4, AF_INET);
	if (h != NULL)
		this->SetUpFromHostent(h);

	return true;
}

void CHostInfo::SetUpFromHostent(const struct hostent* pHost)
{
	int i = 0;
	if (pHost->h_name != NULL)
		this->SetHostName(pHost->h_name);

	while (pHost->h_aliases[i] != NULL)
	{
		this->AddAlias(pHost->h_aliases[i]);
		i++;
	}

	i = 0;
	while (pHost->h_addr_list[i] != NULL)
	{	
		struct in_addr ia;
		char   buffer[255];

		inet_ntop(AF_INET, (in_addr*)pHost->h_addr_list[i], buffer, 255);
		this->AddAddress(buffer);
		i++;
	}
}

CHostInfo::~CHostInfo(void)
{
	this->Clear();
}
