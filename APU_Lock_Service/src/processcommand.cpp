/*
 * process_command.cpp
 *
 *  Created on: Aug 26, 2019
 *      Author: mark
 */


extern char MessageFromSocket[1024];
extern void log_Function(char *log_message);

void processcommand(void)

{

	if (strcmp(processcommand,"shutdown",8) == 0 ) SignalHandler(1);

	return;
}
