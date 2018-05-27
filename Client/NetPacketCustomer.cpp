#include "stdafx.h"
#include "NetPacketCustomer.h"

CNetPacketCustomer::CNetPacketCustomer(void)
{
}

CNetPacketCustomer::~CNetPacketCustomer(void)
{
}

void CNetPacketCustomer::CreateBuffer()
{
	m_pData = new CNP_CUSTOMER;
	memset(m_pData, 0, sizeof(CNP_CUSTOMER));

	m_pData->msgId = PACKET_ID_CUSTOMER;
}

long CNetPacketCustomer::GetRawDataLen() const
{
	return sizeof(CNP_CUSTOMER);
}

void CNetPacketCustomer::SetId(int id)
{
	if (!m_pData)
		CreateBuffer();

	reinterpret_cast<CNP_CUSTOMER*>(m_pData)->Id = id;
}

void CNetPacketCustomer::SetFirstName(const char * name)
{
	if (!m_pData)
		CreateBuffer();

	if (name == NULL)
		return ;

	strcpy(reinterpret_cast<CNP_CUSTOMER*>(m_pData)->FirstName, name);
}

void CNetPacketCustomer::SetLastName(const char * name)
{
	if (!m_pData)
		CreateBuffer();

	if (name == NULL)
		return ;

	strcpy(reinterpret_cast<CNP_CUSTOMER*>(m_pData)->LastName, name);
}

void CNetPacketCustomer::SetBorn(char day, char month, int year)
{
	if (!m_pData)
		CreateBuffer();

	reinterpret_cast<CNP_CUSTOMER*>(m_pData)->Born.day   = day;
	reinterpret_cast<CNP_CUSTOMER*>(m_pData)->Born.month = month;
	reinterpret_cast<CNP_CUSTOMER*>(m_pData)->Born.year  = year;
}