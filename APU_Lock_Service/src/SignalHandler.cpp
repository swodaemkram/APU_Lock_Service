/*
 * SignalHandler.cpp
 *
 *  Created on: Aug 22, 2019
 *      Author: mark
 */

#include <iostream>
#include <cstdio>
#include <string.h>

extern char pid_loc_file_name[250];
extern char APULock_command_sock_name[250];
extern char APULock_response_sock_name[250];

void log_Function(char *log_message);




void SignalHandler(int signum)
{

			remove(pid_loc_file_name);
			remove(APULock_command_sock_name);
			remove(APULock_response_sock_name);
			char log_message[250] = {};
			sprintf(log_message,"terminate signal received shutting APU Lock service down\n");
			log_Function(log_message);
			memset(log_message,0,250);
			exit(0);
}
