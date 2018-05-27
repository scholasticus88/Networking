#ifndef __NET_PACKET__

#define __NET_PACKET__

#include "CommonIncludes.h"

#define PACKET_ID_CUSTOMER 1

class CNetPacket
{
public:
	typedef struct 
	{
		int msgId;
	} CNP_MESSAGE;

	CNP_MESSAGE* m_pData;	

public:
	CNetPacket(void);
	virtual ~CNetPacket(void);
	
	const char* GetRawData() const;
	virtual long GetRawDataLen() const = 0;

	virtual void CreateBuffer() = 0;
};

#endif