
/*
 * Networking code to talk to the pandaboard targeting system
 */

#ifndef TARGETINGSYSTEMCLIENT_H
#define TARGETINGSYSTEMCLIENT_H

#include "VxWorks.h"
#include "sockLib.h"
#include "inetLib.h"
#include "stdioLib.h"
#include "Timer.h"


class TargetingSystemClient
{
public:
	
	TargetingSystemClient();
	~TargetingSystemClient();
	
	bool Is_Connected(void) { return m_Connected; }
	bool Connect(const char * server,unsigned short port);
	bool Update();
	bool Send(char * data_out,int size);
	
	void Set_Turret_Angle(float a) { m_TurretAngle = a; }
	float Get_Target_Distance() { return m_TargetDistance; }
	float Get_Target_Angle() { return m_TargetAngle; }
	float Get_Target_Bank() { return m_BankAngle; }
	
protected:
	
	void Handle_Incoming_Data(char * data,int size);
	void Handle_Command(char *data);
	void Handle_Target(char *data);
	float m_TurretAngle;
	float m_TargetDistance;
	float m_TargetAngle;
	float m_BankAngle;
	
	// networking
	int m_SocketHandle;
	bool m_Connected;
	bool gotdata;
	Timer *m_CommTimer;
};



#endif //TARGETINGSYSTEMCLIENT_H
