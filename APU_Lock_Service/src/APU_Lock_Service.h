/*
 * APU_Lock_Service.h
 *
 *  Created on: Aug 22, 2019
 *      Author: mark
 */

#ifndef APU_LOCK_SERVICE_H_
#define APU_LOCK_SERVICE_H_


#include <time.h>
#include <unistd.h> // for bzero
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SerialStream.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdint>
#include <bits/stdc++.h>    // for strcpy
#include <array>            // for array copy
#include <iostream>

using namespace std;
/*
=========================================================================================================
All Available Functions
=========================================================================================================
*/

void log_Function(char *log_message); //provides logging service for APU_Lock_Service
void SignalHandler(int signum);
string GetResponse();
void mssleep(int micros);
int LockLock(void);
void SendChar(char ochr);
void SendString(string str);
string genkey(string challenge);
char* rot(char b[], char seed);
int UnlockLock(void);
void domain_socket_server(void);
void processcommand(void);
void getLockDoorSensors(void);
/*
=========================================================================================================
End Of Available Functions
=========================================================================================================
Global Variables
=========================================================================================================
*/
char comm_port[250]; //Comm Port passed by command line
char pid_loc_file_name[250];
char APULock_command_file_name[250];
int procnumber;
/*
=========================================================================================================
End Of Global Variables
=========================================================================================================
*/


#endif /* APU_LOCK_SERVICE_H_ */
