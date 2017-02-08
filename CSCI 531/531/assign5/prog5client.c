#include"prog5client.h"

/* for accessing array of commands */
extern char * cmds[];

/* for setting errno for PError */
extern int errno;

/* globally accessible buffer for retrieving data from server */
char servin[MAXLINE];

/*********************************************************************
* prog5client
*
* Description:
* This function is called by FTP.c when a connection to a server is
* to be opened. The argument specified by the user is sent through
* using serverIP and argc. If the user wishes to completely exit the
* FTP client program, this function issues a return value of 1.
*
* Arguments:
*
*   serverIP - the IP specified by the user of the server to connect
*   argc     - the number of arguments entered when the user typed 
*              open (including the open command itself)
*
* Return values:
*   Returns 1 in situations where user wishes to fully close the FTP
*   client program.
*   Returns 0 when the user only wishes to exit the prog5client 
*   function.
*
*********************************************************************/

int prog5client(char * serverIP, int argc)
{
  int sockfd, rval, numargs;
  cmd_t cmdval;
  struct sockaddr_in servaddr;
  char usrin[MAXLINE], cmd[MAXLINE], arg1[MAXLINE], arg2[MAXLINE];
  
  /* if no argument given to open, request one */
  if(argc < 2) {
  	if(prompt_user(OPEN_PROMPT, serverIP, MAXLINE, OPEN_ERROR)) return 0;
  }
  
  /* create the socket */
  sockfd = Socket(AF_INET, SOCK_STREAM, 0);
  
  /* set up the servaddr structure */
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(MY_SERV_PORT);
  rval = Inet_pton(AF_INET, serverIP, &servaddr.sin_addr);
  if(rval <= 0) return (0);
  
  /* establish the connection actively */
  rval = Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
  if( rval < 0 ) return (0);
  else fprintf(stdout, "Connected to %s.\n", serverIP);
  
  fprintf(stdout, FTP_PROMPT);
  while(Fgets(usrin, MAXLINE, stdin)){

		remove_newline(usrin);

		numargs = sscanf(usrin,"%s %s %s", cmd,arg1,arg2);

		/* check command requested by the user */
		cmdval = check_cmd(cmd);
		switch(cmdval) {
			case CMD_BYE: case CMD_QUIT:
				fprintf(stdout,"Goodbye.\n");
				Close(sockfd);
				return 1;
				break;
			case CMD_DISCONNECT:
				fprintf(stdout, "Goodbye.\n");
				Close(sockfd);
				return 0;
				break;
			case CMD_HELP:
				do_help(arg1, numargs);
				break;
			case CMD_LCD:
				do_LCD(arg1, numargs);
				break;
			case CMD_MKDIR:
				if(cli_mkdir(sockfd, arg1, numargs)) PError(arg1);
				break;
			case CMD_RMDIR:
				if(cli_rmdir(sockfd, arg1, numargs)) PError(arg1);
				break;
			case CMD_CDUP:
				if(cli_cdup(sockfd)) PError(CHDIR_FAILED);
				break;
			case CMD_PWD:
				cli_pwd(sockfd);
				break;
			case CMD_OPEN:
				fprintf(stdout, "Already connected to %s, use close first.\n", serverIP);
				break;
			case CMD_ERROR:
				fprintf(stdout, INVALID_CMD);
				break;
			case CMD_BLANK:
				break;
			case CMD_CD:
				if(cli_cd(sockfd, arg1, numargs)) PError(arg1);
				break;
			case CMD_DELETE:
				if(cli_delete(sockfd, arg1, numargs)) PError(arg1);
				break;
			case CMD_RENAME:
				if(cli_rename(sockfd, arg1, arg2, numargs)) PError(RENAME_FAILED);
				break;
			default:
				fprintf(stdout, "Error interpreting command!\n");
		}
	
		fprintf(stdout, FTP_PROMPT);
 	} /* end while */
 	return 0;
}

/*********************************************************************
* cli_rename
*
* Description:
* This function handles the responsibility of sending the rename 
* command to the server and then interpreting the response sent
* by the server.
*
* Arguments:
*  sockfd  - the connection socket for the server
*  arg1    - the name of the file to be renamed
*  arg2    - the target name of the file to be renamed
*  numargs - the number of arguments entered by the user
*
* Return values:
* This function returns the value of errno which is taken from
* the server. This value is zero if there is no error.
*
*********************************************************************/

int cli_rename(int sockfd, char * arg1, char * arg2, int numargs)
{
	bool errval;
	
	/* if no arguments entered by user, prompt for them */
	if(numargs < 3) {
		if(numargs == 1) {
			if(prompt_user(RENAME_PROMPT1, arg1, MAXLINE, RENAME_ERROR)) return 0;
		}
		if(prompt_user(RENAME_PROMPT2, arg2, MAXLINE, RENAME_ERROR)) return 0;
	}
	
	/* send command and get response */
	send_cmd(sockfd, CMD_RENAME, arg1, arg2);
	errval = get_response(sockfd);
	if(!errval) {
		fprintf(stdout,"File exists, ready for destination name\n");
		fprintf(stdout,"RNTO command successful.\n");
	}
	return errval;
}

/*********************************************************************
* cli_delete
*
* Description:
* This function handles the responsibility of sending the delete 
* command to the server and then interpreting the response sent
* by the server.
*
* Arguments:
*  sockfd  - the connection socket for the server
*  arg     - the name of the file to be deleted
*  numargs - the number of arguments entered by the user
*
*********************************************************************/

int cli_delete(int sockfd, char * arg, int numargs)
{
	bool errval;
	/* make sure argument is valid */
	if(numargs == 1) {
		if(prompt_user(DELETE_PROMPT, arg, MAXLINE, DELETE_ERROR)) return 0;
	}

	/* send command and get response */
	send_cmd(sockfd, CMD_DELETE, arg, NULL);
	errval = get_response(sockfd);
	if(!errval) {
		fprintf(stdout,"DELE command successful.\n");
	}
  return errval;
}

/*********************************************************************
* cli_cd
*
* Description:
* This function handles the responsibility of sending the cd 
* command to the server and then interpreting the response sent
* by the server.
*
* Arguments:
*  sockfd  - the connection socket for the server
*  arg     - the name of the directory to switch to
*  numargs - the number of arguments entered by the user
*
*********************************************************************/

int cli_cd(int sockfd, char * arg, int numargs)
{
	bool errval;
	
	/* make sure argument is valid */
	if(numargs == 1) {
		if(prompt_user(CD_PROMPT, arg, MAXLINE, CD_ERROR)) return 0;
	}
		
	/* send command and get response */
	send_cmd(sockfd, CMD_CD, arg, NULL);
	errval = get_response(sockfd);
	
	if(!errval) {
		fprintf(stdout,"CWD command successful.\n");
	} else {
		/* otherwise get interpolated path from server for
		   printing by PError */
		Readline(sockfd, arg, MAXLINE);
		remove_newline(arg);
	}
		
	return errval;
}

/*********************************************************************
* cli_mkdir
*
* Description:
* This function handles the responsibility of sending the mkdir 
* command to the server and then interpreting the response.
*
* Arguments:
*  sockfd  - the connection socket for the server
*  arg     - the name of the directory to create
*  numargs - the number of arguments entered by the user
*
*********************************************************************/

int cli_mkdir(int sockfd, char * arg, int numargs)
{
	bool errval;
	if(numargs == 1) {
		if(prompt_user(MKDIR_PROMPT, arg, MAXLINE, MKDIR_ERROR)) return 0;
	}
	
	send_cmd(sockfd, CMD_MKDIR, arg, NULL);
	errval = get_response(sockfd);
	if(!errval) {
		fprintf(stdout,"MKD command successful.\n");
	}
	return errval;
}

/*********************************************************************
* cli_rmdir
*
* Description:
* This function handles the responsibility of sending the rmdir 
* command to the server and then interpreting the response.
*
* Arguments:
*  sockfd  - the connection socket for the server
*  arg     - the name of the directory to remove
*  numargs - the number of arguments entered by the user
*
*********************************************************************/

int cli_rmdir(int sockfd, char * arg, int numargs)
{
	bool errval;
	if(numargs == 1) {
		if(prompt_user(RMDIR_PROMPT, arg, MAXLINE, RMDIR_ERROR)) return 0;
	}
	
	send_cmd(sockfd, CMD_RMDIR, arg, NULL);
	errval = get_response(sockfd);
	if(!errval) {
		fprintf(stdout,"RMD command successful.\n");
	}
	return errval;
}

/*********************************************************************
* prompt_user
*
* Description:
* Prompts the user for an argument that wasn't entered.
*
* Arguments:
*   prompt  - a character string to use to prompt the user
*   buf     - buffer to store user's result
*   bufsize - size of the buffer for user's response
*   errmsg  - an error message that is printed if user does not
*             enter any response except a newline
*
* Return values:
* Returns 0 on successful input.
* Returns 1 on failed input.
*
*********************************************************************/

int prompt_user(const char * prompt, char * buf, int bufsize, 
				const char * errmsg)
{
	int n;
	fprintf(stdout, prompt);
	Fgets(buf, bufsize, stdin);
	if(buf[0] == '\n') {
		fprintf(stderr, errmsg);
		return 1;
	} else {
		/* remove newline from user input */
		n = strlen(buf);
		if(n > 0) buf[n-1] = 0;
		else return 1;
	}
	return 0;	
}

/*********************************************************************
* cli_cdup
*
* Description:
* This function handles the responsibility of sending the CDUP 
* command to the server and then interpreting the response.
*
* Arguments: 
*   sockfd - the file descriptor for the connection socket to the
*            server
*
*********************************************************************/

int cli_cdup(int sockfd) 
{
	bool errval;
	send_cmd(sockfd, CMD_CDUP, NULL, NULL);
	errval = get_response(sockfd);
	if(!errval) {
		fprintf(stdout,"CWD command successful.\n");
	}
	return errval;
}

/*********************************************************************
* cli_pwd
*
* Description:
* This function handles the responsibility of sending the pwd
* server, retrieving the working directory sent by the server,
* and then printing the resulting working directory.
*
* Arguments:
*   sockfd - file descriptor used for connection to the server
*
*********************************************************************/

int cli_pwd(int sockfd)
{
	bool errval;
	send_cmd(sockfd, CMD_PWD, NULL, NULL);
	errval = get_response(sockfd);
	
	/* if everything went ok, print the current directory */
	if(!errval) {
		Readline(sockfd, servin, MAXLINE);
		remove_newline(servin);
		fprintf(stdout, "\"%s\" is current directory.\n", servin);
	}
	return errval;
}

/*********************************************************************
* send_cmd
*
* Description:
* Handles the responsibility of sending the command to the 
* server. Assumes that arg1 and arg2 point to valid arguments or
* are null if they are not needed.
*
* Arguments:
*
*   cmdval - the command to send to the server
*   arg1   - argument 1 to send to server
*   arg2   - argument 2 to send to server
*
*********************************************************************/

void send_cmd(int sockfd, cmd_t cmdval, char *arg1, char *arg2)
{
	char sendbuf[MAXLINE];
	
	/* if the command requires arguments send those */
	if(arg1) {
		if(arg2)
			snprintf(sendbuf, MAXLINE, "%s %s %s\n", cmds[cmdval], arg1, arg2);
		else
			snprintf(sendbuf, MAXLINE, "%s %s\n", cmds[cmdval], arg1);	
	} else 
			snprintf(sendbuf, MAXLINE, "%s\n", cmds[cmdval]);
	
	/* now send the command string to the server */
	Writen(sockfd, sendbuf, strlen(sendbuf));
	return;
}

/**********************************************************************
* get_response
*
* Description:
* Gets the errno sent by the server, sets the local errno variable to
* this value, and then returns a value (boolean) to indicated whether 
* or not an error occurred on the server's side.
*
* Arguments:
*  sockfd - the file descriptor for the connection socket to the
*           server
*
* Return Values:
* Returns 1 if the server returned an error.
* Returns 0 if the server indicated the operation was successful.
*
**********************************************************************/

int get_response(int sock_fd)
{
	/* get server's response */
	Readline(sock_fd, servin, MAXLINE);
	
	/* convert it from a string to an int */
	if(sscanf(servin,"%d", &errno)) {
		/*if an error occurred return 1 */
		if(errno) return true;
		return false;
	} else {
		fprintf(stderr, "No errno returned from server!\n");
		errno = 0;
		return false;
	}
}
