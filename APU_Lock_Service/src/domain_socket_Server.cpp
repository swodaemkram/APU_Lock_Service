/*
 * domain_socket_Server.cpp
 *
 *  Created on: Aug 26, 2019
 *      Author: mark
 */


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <netinet/in.h>

using namespace std;

extern int procnumber;
extern string sensor_status;
char APU_Lock_sock_name[250]= {};

void log_Function(char *log_message);
void processcommand(void);

bool Socket_Initialized = false;
char MessageFromSocket[1024] = "";


void domain_socket_server (void)
{

	if(Socket_Initialized) goto RXNOW;
	int sock, msgsock, rval;
	struct sockaddr_un server; //Set up structure for socket
	char buf[1024];				//Buffer

	sock = socket(AF_UNIX, SOCK_STREAM, 0); //setup socket
	fcntl(sock, F_SETFL, O_NONBLOCK); // Set Socket for NON-Blocking
	server.sun_family = AF_UNIX;           //Protocol

	sprintf(APU_Lock_sock_name,"/var/run/APU_LockService%d.socket",procnumber);
	//strcpy(server.sun_path, "mei_command.sock");		//build socket path
	strcpy(server.sun_path, APU_Lock_sock_name);
	bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)); //Bind Socket

    listen(sock, 1); //Listen to socket
    usleep(100000);  //This delay is critical for the operation of the network client
	msgsock = accept(sock, 0, 0); //Accept connection from anyone

	if (msgsock == -1)
	{
		close(msgsock);
		close(sock);
		unlink(APU_Lock_sock_name);
		return;
	}


Socket_Initialized = true;
return;

//------------------------------------------Socket Ready for connection ----------------------------------------
//------------------------------------------Received Connection ------------------------------------------------
RXNOW:

	//printf("msgsock = %d\n",msgsock);//debug
    bzero(buf, sizeof(buf));      //Zero out buffer
    rval = read(msgsock, buf, 1024); //Read from the socket
    if(rval > 0 )
	{
	char log_message[250];
    memset(log_message,0,250);
	sprintf(log_message,"client connected.....");
	log_Function(log_message);
	strcpy(MessageFromSocket,buf);
	memset(buf,0,1025);
	if(strlen(MessageFromSocket) != 0)
		{
			//printf("message from Domain socket = %s\n",MessageFromSocket);//DEBUG
			sprintf(log_message,"Command %s was Received By APU_Lock Service instance %d",MessageFromSocket,procnumber);
			log_Function(log_message);
		}
	processcommand();

	if (sensor_status.size() <= 0)
			{
				sensor_status = "ok";
			}

	write(msgsock,sensor_status.c_str(),7);
	sensor_status = "";
	Socket_Initialized = false;
	close(msgsock);//TESTING
	close(sock);//TESTING
	unlink(APU_Lock_sock_name);//TESTING
	return;
	}


return;
//------------------------------------------End of Connection---------------------------------------------

}





