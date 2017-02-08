/*******************************************************************
* Brandon Tweed        *  prog5util.h *
* CSCI 531             ****************
* Assignment 5
* 4/25/2005
*
* Description:
* This file contains implementations of utility functions used by
* FTP.c, prog5client.c, and prog5server.c.
*
********************************************************************/

#include"prog5util.h"

/*************************************
* Commands (entered by user)
*************************************/

char * cmds[] = 
	{"", "bye", "cd", "cdup", "close", "delete", "help", "lcd", "mkdir",
	"open", "pwd", "rename", "rmdir", "disconnect", "?", "quit"};

/*************************************
* Command Help Messages
*************************************/

char * help[] = 
	{ "", "terminate ftp session and exit\n",
		"change remote working directory\n",
		"change remote working directory to parent directory\n",
		"terminate ftp session\n",
		"delete remote file\n",
		"print local help information\n",
		"change local working directory\n",
		"make directory on the remote machine\n",
		"connect to remote ftp\n",
		"print working directory on remote machine\n",
		"rename file\n",
		"remove directory on the remote machine\n",
		"?Invalid help command\n",
		"terminate ftp session\n",
		"print local help information\n"
		"?Invalid help command "
		"terminate ftp session and exit\n"
};

/*********************************************************************
* check_cmd
*
* Checks if a given string contains a valid FTP command. If a
* valid command is found, the corresponding command type value for
* that command is returned. If not, CMD_ERROR is returned.
*
* Arguments
*  incmd - a character string containing the command to be evaluated
*  cmd_t - the value of the command found within the string, or
*          CMD_ERROR if no valid command found.
*
*********************************************************************/

cmd_t check_cmd(char * incmd)
{
	cmd_t i;
	for(i=CMD_BLANK; i<= CMD_QUIT; i++)
		if(!strcmp(incmd,cmds[i])) return i;
	return CMD_ERROR;
}

/*********************************************************************
* do_LCD
*
* This function is used to implement the LCD command.
* Works like change directory on the local machine except that if
* no directory is specified by the user it will change the
* directory to the user's home directory.
*
* Arguments:
*   arg     - the directory to change to
*   numargs - the number of arguments entered by the user
*
*********************************************************************/

void do_LCD(char * arg, int numargs)
{
	int rval;
	char lcdbuf[MAXLINE];
	
	/* if string was interpolated, replace original incoming argument */
	if(interpolate_tilde(lcdbuf, MAXLINE, arg)) strcpy(arg, lcdbuf);

	if( numargs == 2 ) {
		rval = Chdir(arg);
	} else {
		rval = Chdir(Getenv(HOME_VARIABLE));
	}
  
	if(rval >= 0) {
		if(Getcwd(lcdbuf,MAXLINE))
			fprintf(stdout,"Local directory now %s\n",lcdbuf);
	}
}

/*********************************************************************
* do_help
*
* Prints the help information for a specific FTP command, or the
* list of available FTP commands if no command is specified.
*
* Arguments:
*   arg     - a string containing the name of the command to provide
*             help for. If this is null the entire list of commands
*             will be displayed.
*   numargs - the number of arguments entered by the user
*
*********************************************************************/
void do_help(char * arg, int numargs)
{
	cmd_t cmdval;
	int i;
	if( numargs == 1 ) {
	for(i=1; i < NCMDS; i++) {
		if( ((i-1) % NCOLS) == 0 ) fprintf(stdout,"\n"); 
			fprintf(stdout,"%-15s",cmds[i]);
			fprintf(stdout,"\t");
		}
		fprintf(stdout,"\n");
	} else {
		cmdval = check_cmd(arg);
		if(cmdval == CMD_ERROR) {
			fprintf(stdout,help[cmdval]);
			fprintf(stdout,"%s\n",arg);
		} else {
			fprintf(stdout,"%s\t",arg);
			fprintf(stdout,help[cmdval]);
		}
	}
}

/*********************************************************************
* interpolate_tilde
*
* Description:
* Responsible for taking a path with a tilde in it and substituting
* the home directory path for the tilde.
*
* Arguments:
*  dest     - the destination buffer for the interpolated path
*  maxdest  - size of the destination buffer
*  src      - the source string to be interpolated
*
* Return values:
* Returns 1 if value was interpolated.
* Returns 0 if no interpolation took place.
*
*********************************************************************/  
     
int interpolate_tilde(char * dest, int maxdest, const char * src)
{ 
	char * home_path;

	if(src[0] == '~') {
		home_path = getenv(HOME_VARIABLE);
		if(!home_path) return 0;
		switch(src[1]) {
			case '/':
				snprintf(dest, maxdest, "%s/%s", home_path, src+2);
				break;
			default:
				snprintf(dest, maxdest, "%s%s", home_path, src+1);
		}	
		// fprintf(stdout, "Interpolated string: *%s*\n", dest);
		return 1;
	} 
	return 0;
}

/*********************************************************************
* remove_newline
*
* Description:
* Removes a newline from the end of a string by replacing it with
* the null character.
*
* Arguments:
*  buf      - the buffer containing a string with a newline at end
*
* Note: Assumes that the programmer knows what he/she is doing and
*       only invokes the function for strings with newline chars.
*
*********************************************************************/ 

void remove_newline(char * buf){
	int n;
	n = strlen(buf);
	if(n > 0) {
		buf[n-1] = 0;
	}
	return;
}

