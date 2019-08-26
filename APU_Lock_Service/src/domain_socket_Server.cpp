/*
 * domain_socket_Server.cpp
 *
 *  Created on: Aug 26, 2019
 *      Author: mark
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <netinet/in.h>

extern int procnumber;
char APU_Lock_sock_name[250] = {0};

void log_Function(char *log_message);

void domain_socket_server (void)
{

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
		//perror("socket"); //DEBUG should say Resource Temporarily Unavailable
		//WORKING HERE///////////////////////////////////////////////////////////////////////
		close(msgsock);
		close(sock);
		unlink(APU_Lock_sock_name);
		//WORKING HERE//////////////////////////////////////////////////////////////////////
		return;
	}

	//printf("msgsock = %d\n",msgsock);//debug
    bzero(buf, sizeof(buf));      //Zero out buffer
    rval = read(msgsock, buf, 1024); //Read from the socket
    //rval = recv(msgsock,buf,1024,0);   // Recv from socket same as ^

    if(rval > 0 )
	{
	//printf("--> %s\n ", buf); //Print Results DEBUG

	char log_message[250];
    memset(log_message,0,250);
	sprintf(log_message,"client connected to send command");
	log_Function(log_message);

	//WORKING HERE///////////////////////////////////////////////////////////////////////////////
/*
	if (strlen(buf) <= 3)
	{
		close(msgsock);
		close(sock);
		unlink(APU_Lock_sock_name);
		return;
	}
*/

	if(strncmp(buf,"reset",5)==0 || strncmp(buf,"verify",6) ==0 || strncmp(buf,"stack",5) ==0 || strncmp(buf,"idle",4) ==0 || strncmp(buf,"appver",6)==0 ||
 strncmp(buf,"bootver",7)==0 || strncmp(buf,"model",5) ==0 || strncmp(buf,"serial",6) == 0 || strncmp(buf,"varname",7)==0|| strncmp(buf,"stop",4)==0)
    {
	strcpy(APU_Lock_sock_name,buf);
	close(msgsock);
	close(sock);
	unlink(APU_Lock_sock_name);
	return;
    }

    memset(log_message,0,250);
    sprintf(log_message,"client sent invalid command");
    log_Function(log_message);

 //WORKING HERE///////////////////////////////

    //close(msgsock);
    //close(sock);
    //unlink(APU_Lock_sock_name);
	return;
	}

    return;

}
