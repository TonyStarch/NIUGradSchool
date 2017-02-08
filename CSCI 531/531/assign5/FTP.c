/*******************************************************************
* Brandon Tweed        *  FTP.c *
* CSCI 531             **********
* Assignment 5
* 4/25/2005
*
* Description:
* This file contains the main program for the client portion of the
* FTP assignment. In particular, this file contains a command prompt
* that runs while the client is not connected to the server and 
* handles functions such as "help" and "lcd". When the user wishes
* to establish a connection, the prog5client function, contained
* within prog5client.c is called to handle the duties of opening the
* connection, sending commands to the server, and closing the 
* connection. When a user disconnects they may either come back to
* main, or exit from the program completely depending upon the
* program that is used.
*
********************************************************************/

/* include this for utility functions */
#include"prog5util.h"

/* for the function that does "open" */
#include"prog5client.h"

int main(int argc, char ** argv)
{ 
	char usrin[MAXLINE];
	char cmd[MAXLINE], arg1[MAXLINE], arg2[MAXLINE];
	int  cmdval, numargs, exitval;

  /* prompt user for input */
  fprintf(stdout, FTP_PROMPT);

	while(Fgets(usrin, MAXLINE, stdin)) {
		
		remove_newline(usrin);
		
		/* break arguments into separate tokens */
		numargs = sscanf(usrin,"%s %s %s", cmd, arg1, arg2);
		
		/* check command requested by the user */
		cmdval = check_cmd(cmd);
		switch(cmdval) {
			case CMD_BYE: case CMD_QUIT:
				exit(0);
				break;
			case CMD_HELP:
				do_help(arg1,numargs);
				break;
			case CMD_QM:
				do_help(arg1,numargs);
				break;
			case CMD_LCD:
				do_LCD(arg1, numargs);
				break;
			case CMD_OPEN:
				exitval = prog5client(arg1, numargs);
				if(exitval) exit(0);
				break;
			case CMD_BLANK:
				break;
			case CMD_ERROR:
				fprintf(stdout, INVALID_CMD);
				break;
			default:
				fprintf(stdout, "Not connected.\n");
		}
		fprintf(stdout, FTP_PROMPT);
	}
	exit(0);
}






