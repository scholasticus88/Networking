#include "NetPacket.h"

CNetPacket::CNetPacket(void):
	m_pData(NULL)
{
}

CNetPacket::~CNetPacket(void)
{
	if (m_pData)
		delete m_pData;
}

const char* CNetPacket::GetRawData() const
{
	return (const char*)this->m_pData;
}