//=================================================================================================================
// Name        : APU_Lock_Service.cpp
// Author      : Mark Meadows
// Version     : 00:00:03
// Copyright   : Copyright (c) 2019 Mark Meadows
// Description : APU_Lock_Driver_Service in C++, Ansi-style
//================================================================================================================

/*
===================================================================================================================
sensors
RC0 - bolt sensor
RC1 - exit sensor
RC2

ASCENT LOGK
    S   OK 0 1 1 0  - get sensors OK RC0 RC1 RC2 0;
    a|n OK 5B156A0A - get serial
    r   OK 0 0      - get exit sensor + bolt OK RC1 RC0
    N|A             - set new serial number
    s   OK 00 t=0;  - get status
    f               - get firmware version
    D               - lock (de-energize)

    k   OK 30       - get challenge key
    generate response and then either P or E as below
    "P "+info.get("serial")+" "+genkey(s.substring(3)                   // Request boot loader mode
    "P 5B156A0A " + genkey

    "E "+etime+ " " +genkey - response for challenge key, etime="10"    // energize (unlock lock)

sensors
element 1 = lock        1=lock open
element 2 = door
====================================================================================================================
*/

#include <iostream>
#include "APU_Lock_Service.h"


using namespace std;
using namespace LibSerial;

SerialStream my_serial;

#define CODE_LENGTH     10
#define KEY "7578649673"

string unlock_time="10";      // lock open time in seconds

char master[10][10] ={
        {1,2,3,4,5,6,7,8,9,0},
        {3,4,5,6,7,8,9,0,1,2},
        {6,7,8,9,0,1,2,3,4,5},
        {9,0,1,2,3,4,5,6,7,8},
        {4,5,6,7,8,9,0,1,2,3},
        {7,8,9,0,1,2,3,4,5,6},
        {8,9,0,1,2,3,4,5,6,7},
        {0,1,2,3,4,5,6,7,8,9},
        {2,3,4,5,6,7,8,9,0,1},
        {5,6,7,8,9,0,1,2,3,4}
    };

char log_message[250] = {};

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

string lock_serial="none";
string firmware="none";
string lock_status;
string lock_type;       // "summit" or "ascent"
string lock_voltage;


my_serial.Open(comm_port);
if (my_serial.good() )
       {
          my_serial.SetBaudRate(SerialStreamBuf::BAUD_115200);
          my_serial.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
          my_serial.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
          my_serial.SetParity(SerialStreamBuf::PARITY_NONE);
          my_serial.SetNumOfStopBits(1);
          //printf("Serial Port Connected\n");
          sprintf(log_message,"Connected to Serial Port");
          log_Function(log_message);
          memset(log_message,0,250);
         }
if (! my_serial.good())
{
	//printf("Serial Port Not Found !\n");
	sprintf(log_message,"Serial Port NOT FOUND !");
	log_Function(log_message);
	memset(log_message,0,250);
	my_serial.Close();
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

	 UnlockLock();

my_serial.Close();
SignalHandler(1);
}
/*
======================================================================================================================
End of APU_Lock Service
======================================================================================================================
*/

    SignalHandler(1);
	my_serial.Close();
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
        mssleep(2);
    }



    int len=strlen(input_buffer);
    //printf("Rcvd: %s --%d chars, last char: %02X\n",input_buffer,len,next_char);
    memset(log_message,0,250);
    sprintf(log_message,"Rcvd: %s --%d chars, last char: %02X",input_buffer,len,next_char);
    log_Function(log_message);
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
LockLock
=========================================================================================================================
*/
int LockLock(void)
{

    string resp;
    char send_char='D';
    SendChar(send_char);
    resp=GetResponse();
    //printf("RESP: %s\n",resp.c_str() );
    memset(log_message,0,250);
    sprintf(log_message,"Response From Lock %s",resp.c_str());
    log_Function(log_message);
    return 1;


}
/*
=========================================================================================================================
End of LockLock
=========================================================================================================================
UnlockLock
=========================================================================================================================
*/
int UnlockLock(void)
{

    char send_char;
    string resp;
    string key;
    string snd;

    //printf("\nUnlocking the lock");
    memset(log_message,0,250);
    sprintf(log_message,"Unlock Command Sent to Lock...",resp.c_str());
    log_Function(log_message);


    send_char='k';
    //printf("\nSending %c\n",send_char);
    memset(log_message,0,250);
    sprintf(log_message,"Sending %c",send_char);
    log_Function(log_message);

    SendChar(send_char);
    resp=GetResponse();

    //printf("Challenge Key: %s\n",  resp.c_str());   //Challenge Key: OK 5715393577;
    memset(log_message,0,250);
    sprintf(log_message,"Challenge Key: %s",resp.c_str());
    log_Function(log_message);

/*
        use this section only of the challenge key packet
        |--------|
    "OK 9036540674;"
*/
    //printf("Generating response key\n");
    memset(log_message,0,250);
    sprintf(log_message,"Generating response key... %s",resp.c_str());
    log_Function(log_message);

    key = genkey(resp.substr(3));
    //printf("Response key: %s\n",key.c_str());
    memset(log_message,0,250);
    sprintf(log_message,"Response Key: %s",key.c_str());
    log_Function(log_message);

    snd = "E " +unlock_time+" "+ key +";";
    //printf("Sending:%s\n", snd.c_str() );
    memset(log_message,0,250);
    sprintf(log_message,"Sending: %s",snd.c_str());
    log_Function(log_message);
    SendString(snd);
    mssleep(50);
    resp = GetResponse();
    //printf("Resp: %s\n", resp.c_str() );
    memset(log_message,0,250);
    sprintf(log_message,"Response From Lock %s",resp.c_str());
    log_Function(log_message);

	return 0;
}
/*
=========================================================================================================================
End of Unlock Lock
=========================================================================================================================
Generate key from provided challenge code
=========================================================================================================================
*/

string genkey(string challenge)
{
        unsigned char r = 0;

        char master[] = KEY;    // "7578649673";
        char ctmp[CODE_LENGTH];                              // = new byte[10];
        char code[CODE_LENGTH];
		bzero(ctmp,CODE_LENGTH);
		bzero(code,CODE_LENGTH);

        // strip the ";" off the end
        if (challenge.size () > 0)  challenge.resize (challenge.size () - 1);

        //printf("Challenge: %s[%d]\n",challenge.c_str(), (int) challenge.size() );
        memset(log_message,0,250);
        sprintf(log_message,"Challenge %s[%d]",challenge.c_str(), (int) challenge.size());
        log_Function(log_message);




        // convert string to byte array
        char salt[12];
        bzero(salt,12);

        for(int i = 0; i < CODE_LENGTH; i++)
            salt[i] = challenge[i] - 0x30;

        for(int i = 0; i < CODE_LENGTH; i++)
            master[i] = master[i] - 0x30;

        for(int i = 0; i < CODE_LENGTH; i++)
            ctmp[i] = (char) ((salt[i] + master[i]) % 10);

        for(int i = 0; i < 5; i++)
        {
            char *x = rot(ctmp, r);
            for (int i=0; i < CODE_LENGTH; i++)
                ctmp[i] = x[i];
            r = ctmp[9];
        }

        for(int i = 0; i < CODE_LENGTH; i++)
        {
            code[i] = (unsigned char) ('0' + ctmp[i]);
        }

        string str;
        for (int i=0; i<CODE_LENGTH; i++)
            str += code[i];
        return str;

}

/*
=========================================================================================================================
End Generate key from provided challenge code
=========================================================================================================================
rot Function
=========================================================================================================================
*/

char* rot(char b[], char seed)
{
    char a[10];
    for (int i=0; i < CODE_LENGTH; i++)
        a[i]=b[i];
    b[0] = master[seed][a[0]];
    for(int i = 1; i < CODE_LENGTH; i++) {
        b[i] = master[b[i-1]][a[i]];
    }
    return &b[0];
}
/*
=========================================================================================================================
End rot Function
=========================================================================================================================
*/























