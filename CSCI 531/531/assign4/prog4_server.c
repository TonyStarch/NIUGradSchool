
/********************************************************************
*
* File:       prog4_server.c
* Author:     Brandon Tweed
* Date:       3/23/2005
* Course:     CSCI 531
* Instructor: Ibrahim Onyuksel
*
* Description:
* This file implements the server program portion of assignment 4.
* The server waits for a client to connect, then when a connection
* is received the server creates a child process to handle the 
* connection. 
* This child process enters a loop, gets the title of a book from the
* client, searches the database for an entry with a corresponding
* author. If an author is found the name of the author is sent back
* to the client. When the client stops sending data (EOF), the child
* terminates and sends a signal back to the server's parent process.
* This signal is received by the signal handler sig_chld.
*
* Notes:
*
* * The title received from the client has a newline character present
*   which must be removed before it is used for searching the
*   data base file for the matching author.
*
* * The author returned the user will have a newline appended to it
*   so that the client's call to Readline will succeed.
*
* main() variables:
* listenfd   - file descriptor for listening socket
* connfd     - file descriptor for connection socket
* clilen     - length of the client's sockaddr structure (cliaddr)
* childpid   - returned process id from fork
* servaddr   - server's sockaddr structure
* cliaddr    - client's sockaddr structure
*
********************************************************************/

#include"prog4_server.h"

int main(int argc, char ** argv)
{
  int listenfd, connfd;
  socklen_t clilen;
  pid_t childpid;
  struct sockaddr_in servaddr, cliaddr;
  
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
  		str_respond(connfd);
  		Close(connfd);
  		exit(0);
  	}
  	Close(connfd); /* parent close connected socket */
	
  } /* end while */
	
  printf("Closing fds\n");
  Close(listenfd);
  exit(0);
}

/********************************************************************
* str_respond
*
* Description:
* Function responsible for handling the actions of a child process
* Opens the database file, searches for authors as long as input is
* still being sent from the client, and sloses the database when
* it's done.
*
* Arguments:
*    db     - file descriptor for the child's connection socket
*
* Variables:
*    n      - used to hold the length of strings 
*    bufin  - this buffer holds input coming from the client program
*             which is then used for conducting the search
*    bufout - this is the buffer where output is written before being
*             sent to the client program
*    db     - this is a file pointer used when the data base
*             file is opened, read, and then closed
*
********************************************************************/
void str_respond(int fd)
{
  int    n;
  char   bufin[MAXLINE], bufout[MAXLINE]; 
  FILE * db;
	
  db = Fopen(DB_FILE,"r"); /* open the database */
    	
  while ( (n = Readline(fd, bufin, MAXLINE)) ) {
      
    /* remove newline character */
    n = strlen(bufin);
    bufin[n-1] = '\0';
      
  	search_db(db, bufout, bufin);
  		
  	/* send the author name with newline */
  	n = strlen(bufout);
    Writen(fd, bufout, n);
  }
  
  Fclose(db); /* close the database */
  return;
}

/********************************************************************
* search_db
*
* Description:
* Searches the database for the author matching the title, given
* by author and title as arguments.
*
* Arguments:
*    db     - a pointer to the file to be used for the data base
*    author - destination string buffer for the name of the author
*             that matches the title
*    title  - pointer to a string which contains the title to be
*             searched for. will be altered by being converted
*             to lower case for searching.
*
* Variables:
*    found   - flag set to indicate whether or not the matching
*              title was found or not
*    t1,t2   - these are strings used to hold the entries for title
*              and author from each line of the db file
*    inpline - this is the buffer holding an entire line read from
*              the input file, scanned for a title and author which
*              are then put into t1 and t2 respectively
*
********************************************************************/
void search_db(FILE * db, char * author, char * title)
{
	bool found;
	char t1[MAXLINE],t2[MAXLINE];
	char inpline[MAXLINE];
	found = false;	
	
	makelower(title); /* convert client's title to lower case */

	/* do the search */
	while(fgets(inpline, MAXLINE, db) && !found) {
		sscanf(inpline,"%[^:]:%[^:];",t1,t2);
		makelower(t1);
		if(strcmp(t1,title) == 0) /* title found ? */
			found = true;
	}
	
	if (!found) {
		snprintf(author, MAXLINE, "%s\n", UNKNOWN_MSG);
	} else {
		snprintf(author, MAXLINE, "%s\n", t2);
	}
	
	/* reset the file pointer to beginning */
	fseek(db, 0, SEEK_SET);
}

/*********************************************************************
* makelower
* 
* Description: 
* Converts the string supplied as an argument to lower case.
*
* Arguments:
*   s - pointer to the string to be converted to lower case
*
*********************************************************************/
void makelower(char * s) 
{
	char * ptr; /* for moving through the string */
	ptr = s;
	while( *ptr != '\0' ) {
		*ptr = tolower(*ptr);
		ptr++;
	}		
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
