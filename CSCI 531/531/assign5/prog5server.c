/*******************************************************************
* Brandon Tweed        *  prog5server.c *
* CSCI 531             ******************
* Assignment 5
* 4/25/2005
*
* Description:
* This file implements the server portion of the FTP assignment.
* The server listens for a connection, and when a connection from
* a client is established, spawns a child which handles that FTP
* connection. The child process listens for commands sent by the
* client, performs the corresponding commands, and then sends back
* the errno from the system call. In cases where more data needs
* to be sent, the additional data is sent to the user.
*
********************************************************************/

#include"prog5server.h"
#include"prog5util.h"
#include"in_out.h"
#include"wrapper.h"

/* cmds points to an array of strings for commands */
extern char * cmds[];

/* for accessing the array of exit flags so that they
   can be set */   
extern bool exit_flags[];

/* this is global so as to avoid duplicating the same large
   buffer inside many of the server functions */
char bufout[MAXLINE];
    
int main(int argc, char ** argv)
{
  int listenfd, connfd;
  socklen_t clilen;
  pid_t childpid;
  struct sockaddr_in servaddr, cliaddr;
  
  /* set the wrapper exit flags */
  set_flags();
  
  /* create the listening socket */
  listenfd = Socket(AF_INET, SOCK_STREAM, 0);
  
  /* set up servaddr structure */
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(MY_SERV_PORT);
  
  /* bind the listenfd to the port */
  Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
  
  /* move the socket into the LISTEN state */
  Listen(listenfd, LISTENQ);
  
  /* set up server to accept signals from children */
  Signal(SIGCHLD, sig_chld); 
  
  while(true) {
  	clilen = sizeof(cliaddr);
  	connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
  	
  	/* fork and do child tasks */
  	if( ( childpid = Fork() ) == 0 ) {
  		Close(listenfd);
  		ftp_cmd(connfd);
  		Close(connfd);
  		exit(0);
  	}
  	Close(connfd); /* parent close connected socket */
	
  } /* end while */
	
  Close(listenfd);
  exit(0);
}

/*********************************************************************
* ftp_cmd
*
* Description:
* This function perform's the child's duty of receiving a command
* from the client, performing the actions associated with the command
* returning the error number to the user, and then sending any data
* necessary to accompany the error message.
*
* Arguments:
*   fd   - the file descriptor for the connection to the client
*
*********************************************************************/

void ftp_cmd(int fd)
{
	int    numargs;
	cmd_t  cmdval;
	
	char cmd[MAXLINE], bufin[MAXLINE],
	     arg1[MAXLINE], arg2[MAXLINE];
	
	/* while the client keeps sending commands, process them */
	while((Readline(fd, bufin, MAXLINE))) {

		remove_newline(bufin);

		/* tokenize the command sent with newline removed */
		numargs = sscanf(bufin,"%s %s %s", cmd, arg1, arg2);
		
		/* perform actions associated with the command */
		cmdval = check_cmd(cmd);
		switch(cmdval) {
			case CMD_CDUP:
				serv_cdup(fd);
				break;
			case CMD_PWD:
				serv_pwd(fd);
				break;
			case CMD_CD:
				serv_cd(fd, arg1);
				break;
			case CMD_MKDIR:
				serv_mkdir(fd, arg1);
				break;
			case CMD_RMDIR:
				serv_rmdir(fd, arg1);
				break;
			case CMD_DELETE:
				serv_delete(fd, arg1);
				break;
			case CMD_RENAME:
				serv_rename(fd, arg1, arg2);
				break;
			default:
				fprintf(stderr,"Unknown command sent.\n");
		}
	}
	return;
}

/*********************************************************************
* serv_rename
*
* Description:
* Handles performing the duties associated with the rename command.
* (renames the target specified by arg1 to the name specified by arg2)
*
* Arguments:
*   fd   - the file descriptor for the connection to the client
*   arg1 - a character string for the name of the file to rename
*   arg2 - a character string for the name to rename to
*
*********************************************************************/

void serv_rename(int fd, char * arg1, char * arg2)
{
	
	/* create directory with rwx permission for owner */
	rename(arg1, arg2);
	return_errno(fd);
	
	/* reset the errno */
	errno = 0;
}

/*********************************************************************
* serv_delete
*
* Description:
* Handles performing the duties associated with the delete command.
* (deletes the file specified by arg1 on the server side)
*
* Arguments:
*   fd   - the file descriptor for the connection to the client
*   arg1 - a character string for the name of the file to delete
*
*********************************************************************/

void serv_delete(int fd, char * arg1)
{
	/* create directory with rwx permission for owner */
	remove(arg1);
	return_errno(fd);
	
	/* reset the errno */
	errno = 0;
}

/*********************************************************************
* serv_mkdir
*
* Description:
* Handles performing the duties associated with the mkdir command.
* (Creates the directory specified by arg1 on the server side)
*
* Arguments:
*   fd   - the file descriptor for the connection to the client
*   arg1 - a character string for the name of the directory to
*          create
*
*********************************************************************/

void serv_mkdir(int fd, char * arg1)
{
	/* create directory with rwx permission for owner */
	mkdir(arg1, S_IRWXU);
	return_errno(fd);
	
	/* reset the errno */
	errno = 0;
}

/*********************************************************************
* serv_cd
*
* Description:
* Handles performing the duties associated with the CD command
* for the server.
*
* Arguments:
*   fd - the file descriptor for the connection to the client
*
*********************************************************************/

void serv_cd(int fd, char * arg1)
{
	char interpbuf[MAXLINE];
	int rval;
	
	if(interpolate_tilde(interpbuf, MAXLINE, arg1)) strcpy(arg1, interpbuf);
	// fprintf(stdout, "interpolated string was: *%s*\n", interpbuf);
	rval = chdir(arg1);
	return_errno(fd);
	
	/* if error occurred, return the interpolated path name to client */
	if(rval < 0) {
		snprintf(bufout, MAXLINE, "%s\n", interpbuf);
		Writen(fd, bufout, strlen(bufout));
	}
	 
	/* reset the errno */
	errno = 0;
	return;
}

/*********************************************************************
* serv_rmdir
*
* Description:
* Handles performing the duties associated with the rmdir command
* for the server.
*
* Arguments:
*   fd   - the file descriptor for the connection to the client
*   arg1 - the directory specified by the client to remove
*
*********************************************************************/

void serv_rmdir(int fd, char * arg1)
{
	 rmdir(arg1);
	 return_errno(fd);
	 
	 /* reset the errno */
	 errno = 0;
	 return;
}

/*********************************************************************
* serv_cdup
*
* Description:
* Handles performing the duties associated with the CDUP command
* for the server.
*
* Arguments:
*   fd - the file descriptor for the connection to the client
*
*********************************************************************/

void serv_cdup(int fd)
{
	chdir("..");
	return_errno(fd);
	
	/* reset the errno */
	errno = 0;
	return;
}

/*********************************************************************
* serv_pwd
*
* Description:
* Handles performing the duties associated with the PWD command
* for the server.
*
* Arguments:
*   fd - the file descriptor for the connection to the client
*
*********************************************************************/

void serv_pwd(int fd)
{
	char * rval;
	char cwd[MAXLINE];
	
	rval = getcwd(cwd, MAXLINE);
	return_errno(fd);
	
	if(rval) {
		snprintf(bufout, MAXLINE, "%s\n", rval);
		Writen(fd, bufout, strlen(bufout));
	}
	
	/* reset the errno */
	errno = 0;
	return;
}

/*********************************************************************
* return_errno
*
* Description:
* This sends the errno from the previous system call back to the 
* client in order to let the client know if anything went wrong
* (or right).
*
* Arguments:
*   fd    - the file descriptor for the connection to the client
*
*********************************************************************/

void return_errno(int fd)
{
		snprintf(bufout,MAXLINE,"%d\n",errno);
		Writen(fd,bufout,strlen(bufout));
}

/*********************************************************************
* sig_chld
* 
* Description: 
* signal handler for handling the SIG_CHLD signal from the child
* process of the server which is issued upon the termination of a
* child.
*
* Arguments:
*   signo - not used but represents the integer value of the signal
*           which is 18 in this case
* Variables:
*   pid   - the returned pid from waitpid of the process which has
*           terminated or 0 if no processes have terminated
*   stat  - returned status of the child process that has become
*           zombified
*
*********************************************************************/

void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	
	/* set up to catch more child signals */
	Signal(SIGCHLD, sig_chld); 
	while( ( pid = waitpid( -1, &stat, WNOHANG) ) > 0 ) {
	}
}

/*********************************************************************
* set_flags
*
* Description:
* Sets the exit flags for the wrapper functions that should
* terminate in the case that an error occurs.
*
*********************************************************************/

void set_flags(void)
{
	exit_flags[SOCKET]    = true;
	exit_flags[LISTEN]    = true;
	exit_flags[BIND]      = true;
	exit_flags[ACCEPT]    = true;
	exit_flags[INET_PTON] = true;
	exit_flags[FORK]      = true;
	exit_flags[READLINE]  = true;
	exit_flags[WRITEN]    = true;
}
