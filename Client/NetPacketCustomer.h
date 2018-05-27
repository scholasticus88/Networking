#ifndef __NET_PACKET_CUSTOMER__

#define __NET_PACKET_CUSTOMER__

#include "stdafx.h"
#include "NetPacket.h"

class CNetPacketCustomer : public CNetPacket
{
public:
	struct CNP_CUSTOMER : public CNP_MESSAGE
	{
		long Id;
		char FirstName[MAX_NAME_LEN];
		char LastName[MAX_NAME_LEN];
		struct DATE {
			char day;
			char month;
			int  year;
		} Born;
	};

	virtual void CreateBuffer(); 

public:
	CNetPacketCustomer(void);
	virtual ~CNetPacketCustomer(void);

	virtual long GetRawDataLen() const;
	void SetId(int id);
	void SetFirstName(const char * name);
	void SetLastName(const char * name);
	void SetBorn(char day, char month, int year);
};

#endif