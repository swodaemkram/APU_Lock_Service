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

SerialStream my_serial;

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
Make Sure We Only Run Once in a Process ID
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
//string portname="/dev/ttyACM0";

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


my_serial.Open(comm_port);
if (my_serial.good() )
            {
                my_serial.SetBaudRate(SerialStreamBuf::BAUD_115200);
                my_serial.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
                my_serial.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
                my_serial.SetParity(SerialStreamBuf::PARITY_NONE);
                my_serial.SetNumOfStopBits(1);
                printf("Serial Port Connected\n");
                sprintf(log_message,"Connected to Serial Port");
                log_Function(log_message);
                memset(log_message,0,250);
            }
if (! my_serial.good())
{
	printf("Serial Port Not Found !\n");
	sprintf(log_message,"Serial Port NOT FOUND !");
	log_Function(log_message);
	memset(log_message,0,250);
	SignalHandler(1);
}

/*
======================================================================================================================
End of Comm Port Setup
======================================================================================================================
Start of APU_Lock Service
======================================================================================================================
*/
while(1) //Service so endless loop
{







}
/*
======================================================================================================================
End of APU_Lock Service
======================================================================================================================
*/

    SignalHandler(1);
	return 0;
}

/*
=======================================================================================================================
Get Response
=======================================================================================================================
*/

string GetResponse()
{

#define BUFFER_SIZE 150
    char next_char ;
    char input_buffer[BUFFER_SIZE] ;
    bzero(input_buffer,BUFFER_SIZE);


	int n=0;
    while (my_serial.rdbuf()->in_avail() >0)
    {
        my_serial.get(next_char);
        input_buffer[n++]=next_char;
        mssleep(1);
    }



    int len=strlen(input_buffer);
    printf("Rcvd: %s --%d chars, last char: %02X\n",input_buffer,len,next_char);
    return string(input_buffer);

}
/*
========================================================================================================================
End of Get Response
========================================================================================================================

=========================================================================================================================
Send String
=========================================================================================================================
 */
void SendString(string str)
{
    my_serial << str;
    my_serial << ";";
    return;
}
/*
=========================================================================================================================
End of Send String
=========================================================================================================================
Send a single Character
=========================================================================================================================
*/
void SendChar(char ochr)
{
    my_serial << ochr;    // write
    mssleep(1);
    my_serial << ";";
    mssleep(1);
    return;
}
/*
==========================================================================================================================
End of send Character
==========================================================================================================================
Sleep micros MS
==========================================================================================================================
*/
void mssleep(int micros)
{
	usleep(micros *1000);
	return;
}
/*
=========================================================================================================================
End of mssleep
=========================================================================================================================
*/
































