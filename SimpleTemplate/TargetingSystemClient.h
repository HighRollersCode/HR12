
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
	void Update();
	bool Send(char * data_out,int size);
	
	void Set_Turret_Angle(float a) { m_TurretAngle = a; }
	float Get_Target_Distance() { return m_TargetDistance; }
	
protected:
	
	void Handle_Incoming_Data(char * data,int size);
	void Handle_Command(char *data);
	void Handle_Target(char *data);
	float m_TurretAngle;
	float m_TargetDistance;
	
	// networking
	int m_SocketHandle;
	bool m_Connected;
	Timer *m_CommTimer;
};



#endif //TARGETINGSYSTEMCLIENT_H
