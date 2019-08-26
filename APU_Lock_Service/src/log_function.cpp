/*
 * log_function.cpp
 *
 *  Created on: Aug 22, 2019
 *      Author: mark
 */

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>

extern int procnumber;

void log_Function(char *log_message)

{

		struct timespec ts;
	    timespec_get(&ts, TIME_UTC);
	    char buff[100];
	    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));

		FILE *mei_service_log = NULL; //declare File Pointer
		char mei_log_file_name[250] = {0};
		sprintf(mei_log_file_name,"/var/log/API_Lock_Service_proc%d.log",procnumber);


		mei_service_log = fopen(mei_log_file_name,"a");
		//printf("\nlog_message = %s\n",log_message);
     	char *Mytime_fmt = " %s.%09ld ";
		char MyTime[27];
		sprintf(MyTime, Mytime_fmt, buff,ts.tv_nsec); //Format and apply data

		int MyTimeLen = 0;
		MyTimeLen = strlen(MyTime);

		fwrite(MyTime,1,MyTimeLen,mei_service_log);
		fwrite(log_message,1,strlen(log_message),mei_service_log);
		fwrite("\n",1,2,mei_service_log);
		fclose(mei_service_log);

		return;

}
