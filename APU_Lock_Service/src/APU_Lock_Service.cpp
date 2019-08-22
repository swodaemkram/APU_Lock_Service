//============================================================================
// Name        : APU_Lock_Service.cpp
// Author      : Mark Meadows
// Version     : 00:00:03
// Copyright   : Copyright (c) 2019 Mark Meadows
// Description : APU_Lock_Driver_Service in C++, Ansi-style
//============================================================================

#include <iostream>
#include "APU_Lock_Service.h"


using namespace std;
using namespace LibSerial;


int main(int argc, char *argv[])

/*
==================================================================================================================
Lets process the service startup commands
==================================================================================================================
*/
{
	if (argv[1] == NULL){
				printf("Comm port must be supplied !\n\n");
				SignalHandler(1);
		}

			strncpy(comm_port,argv[1],strlen(argv[1]));

		if (argv[2] == NULL){
			printf("Must have Instance Number ! \n\n");
			SignalHandler(1);
		}


	procnumber = atoi(argv[2]);

	sprintf(pid_loc_file_name,"/run/APU_Lock_service%d.pid",procnumber);

//printf("pid-file = %s\n",pid_loc_file_name);//DEBUG
/*
===================================================================================================================
Finished processing the service startup commands
===================================================================================================================
Make Sure We Only Run Once
===================================================================================================================
*/
	FILE *pid_lock = NULL;                            	// declare pid lock file

		pid_lock = fopen(pid_loc_file_name,"r");
		if (pid_lock != NULL){
			fclose(pid_lock);
			printf("\n APU_Lock_service is all ready running on this process id\n");
			SignalHandler(1);
		}


		pid_lock = fopen(pid_loc_file_name,"w+");
		if (pid_lock == NULL){
		printf("\ncould not open lock file.\n");
		SignalHandler(1);
		}
		char strpid[6] = {0} ;
		int pid;
		pid = getpid();
		sprintf(strpid,"%d",pid);
		fwrite(strpid,1,sizeof(strpid),pid_lock);
		fclose(pid_lock);										// Close pid lock file
/*
======================================================================================================================
end of run once check
======================================================================================================================
*/

char log_message[250] = {};
sprintf(log_message,"================================================");
log_Function(log_message);
memset(log_message,0,250);
sprintf(log_message,"APU Lock Service started using the %s",comm_port);
log_Function(log_message);
memset(log_message,0,250);
sprintf(log_message,"Ver 01.88.00");
log_Function(log_message);
memset(log_message,0,250);
sprintf(log_message,"================================================");
log_Function(log_message);
memset(log_message,0,250);
signal(SIGTERM,SignalHandler);

/*
======================================================================================================================
Setup Comm Port Here
======================================================================================================================
*/
string portname="/dev/ttyACM0";
char terminator=';';
char send_char='k';
string lock_serial="none";
string firmware="none";
int lock_sensor=-1; // 1
int door_sensor=-1; // 1 = door open
string lock_status;
string lock_type;       // "summit" or "ascent"
string lock_voltage;
const char  serialnum[32]  = "UNDEFINED";
const char  addr[32] = "UNDEFINED";
const char  str_ok[4] = "OK;";
const char  str_err[5] = "ERR;";
const char  str_perr[6] = "PERR;";
const char  str_OpenPar[3] = ")";
string etime="10";      // lock open time in seconds

SerialStream my_serial;















/*
======================================================================================================================
End of Comm Port Setup
======================================================================================================================
*/
















	return 0;
}
