

#include "TargetingSystemClient.h"
#include "VxWorks.h"
#include "sockLib.h"
#include "inetLib.h"
#include "hostLib.h"
#include "ioLib.h"
#include "string.h"
#include "selectLib.h"


TargetingSystemClient::TargetingSystemClient() :
	m_TurretAngle(0.0f),
	m_TargetDistance(0.0f),
	m_TargetAngle(0.0f),
	m_BankAngle(0.0f),
	m_SocketHandle(-1),
	m_Connected(false)
{
	m_CommTimer = new Timer();
}
	
TargetingSystemClient::~TargetingSystemClient()
{
	
}

bool TargetingSystemClient::Connect(const char * server,unsigned short port)
{
	int status;
	sockaddr_in server_addr;
	int addr_size;
	
	// int the select system
	selectInit(2048);
	
	m_SocketHandle = socket(AF_INET,SOCK_STREAM,0);
	if (m_SocketHandle == -1)
	{
		printf("ERROR: Could not create socket!\n");
		return false;
	}
	
	addr_size = sizeof(server_addr);
	memset(&server_addr,0,sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_len = sizeof(server_addr);
	server_addr.sin_port = htons(port);
	
	server_addr.sin_addr.s_addr = inet_addr((char*)server);
	if (server_addr.sin_addr.s_addr == (unsigned int)ERROR)
	{
		server_addr.sin_addr.s_addr = hostGetByName((char*)server);
		if (server_addr.sin_addr.s_addr == (unsigned int)ERROR)
		{
			printf("ERROR: could not resolve server address: %s\n",server);
			return false;
		}
	}
	
	status = connect(m_SocketHandle,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if (status == -1)
	{
		printf("ERROR: connect failed\n");
		close(m_SocketHandle);
		return false;
	}
	
	m_Connected = true;

//	printf("CRio->Panda Connected! server: %s port: %d\n",server,port);
	Send("0\r\n",4);
	m_CommTimer->Reset();
	m_CommTimer->Start();
	return true;
}
	

bool TargetingSystemClient::Update()
{
	
	if (m_Connected == false)
	{
		return false;
	} 
	gotdata = false;
	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(m_SocketHandle,&read_set);
	
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	
	int status = select(m_SocketHandle+1,&read_set,NULL,NULL,&timeout);
	if (status == -1)
	{
		printf("ERROR: select failed. socket: %d errno: %d",m_SocketHandle,errno);
		return false;
	}
	
	if (FD_ISSET(m_SocketHandle,&read_set))
	{
		char msg_buffer[4096];
		
		int bytes_in = recv(m_SocketHandle,msg_buffer,sizeof(msg_buffer),0);
		if (bytes_in = 0)
		{
			m_Connected = false;
	//		printf("Disconnected!");
		}
		else
		{
			msg_buffer[512] = 0; // make sure msg_buffer can never overflow
			//printf("recv: %s\n",msg_buffer);
			Handle_Incoming_Data(msg_buffer,sizeof(msg_buffer));
		}
	}
	
	// Periodically make sure we ask for the current turret angle
	if (m_CommTimer->Get() > 0.1f)
	{
		char buff[256];
		sprintf(buff,"2 %f\n",m_TurretAngle);
		Send(buff,strlen(buff)+1);
		Send("0\r\n",4);

		m_CommTimer->Reset();
	}
	return gotdata;
}

void TargetingSystemClient::Handle_Incoming_Data(char * data,int size)
{
	//printf("Handle_Incoming_Data: %s \n",data);
	
	char *cmd = strtok(data,"\n");
	while(cmd !=NULL)
	{
		Handle_Command(cmd);
		cmd = strtok(NULL,"\n");
	}
	
	//printf("done.\n");
}
void TargetingSystemClient::Handle_Command(char*data)
{
//	printf("Handle_Command: %s\n",data);
	
	switch(data[0])
	{
		case '0':
			Handle_Target(data);
			break;
	}
}
void TargetingSystemClient::Handle_Target(char *data)
{
	sscanf(data,"0 %f %f %f",&m_TargetDistance,&m_TargetAngle,&m_BankAngle);
	gotdata = true;
//	printf("Handle_Target: %f\n",m_TargetDistance);
}

bool TargetingSystemClient::Send(char * data_out,int size)
{
	int status;
	
	if ((m_Connected == true) && (size > 0))
	{
//		printf("send: %s\n",data_out);
		status = send(m_SocketHandle,data_out,size,0);
		if (status == -1)
		{
			printf("ERROR: send failed. status: %d  errno: %d",status,errno);
			return false;
		}
		return true;
	}

	return false;
}


