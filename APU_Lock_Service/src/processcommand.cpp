/*
 * process_command.cpp
 *
 *  Created on: Aug 26, 2019
 *      Author: mark
 */
#include <stdio.h>
#include <string.h>



void SignalHandler(int signum);
void UnlockLock(void);
void getLockDoorSensors(void);



extern char MessageFromSocket[1024];



void processcommand(void)

{

	if (strncmp(MessageFromSocket,"shutdown",8) == 0 )
		{
		SignalHandler(1);
		return;
		}

	if (strncmp(MessageFromSocket,"unlock",6) == 0)
		{
		UnlockLock();
		return;
		}

	if(strncmp(MessageFromSocket,"status",6) == 0)
		{
		getLockDoorSensors();
		return;
		}

	return;
}


