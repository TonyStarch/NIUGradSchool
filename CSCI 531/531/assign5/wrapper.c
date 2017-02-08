/*********************************************************************
* Brandon Tweed
* CSCI 531
* Homework 4
*
* Description:
* This file contains the implementations for the wrapper functions
* defined in wrapper.h. These are used by the main program for 
* opening the socket, getting data from the socket, etc.
*
*********************************************************************/

// Bring in prototypes for wrapper functions
#include"wrapper.h"
// prototypes for I/O functions to be wrapped up 
#include"in_out.h"

/* array of exit flags, initialized to false */
bool exit_flags[NFLAGS] = {false};

/*******************************************************************
* Socket
*
* Creates/allocates a socket using the criteria specified by
* the arguments.
*
* Arguments:
*    domain   - specifies a communication domain
*    type     - used to indicate what type of socket to open
*    protocol - protocol to be used by socket
*
*
*******************************************************************/

int Socket(int domain, int type, int protocol)
{
   int sockfd; // fd for the socket being created/opened
   sockfd = socket(domain, type, protocol);
   if( sockfd < 0 ) {
       PError(SOCKET_FAILED);
       if(exit_flags[SOCKET]) exit(1);
   }
   return sockfd;    
}

/*******************************************************************
* Inet_pton
*
* Converts a text-formate IP address to a presentation-format
*
* Arguments:
*
*    af   - the family of the address to be converted
*    cp   - pointer to text string/address
*    addr - place to store converted result
*
*******************************************************************/

int Inet_pton(int af, const char * cp, void * addr)
{
	int rval; // return value for inet_pton
	rval = inet_pton(af, cp, addr);
	if( rval < 0 ) {
		PError(INET_PTON_FAILED);
		if(exit_flags[INET_PTON]) exit(1);
		return rval;
	}
	else if( rval == 0 ){
		fprintf(stderr,INET_PTON_INVALID);
		if(exit_flags[INET_PTON]) exit(1);
		return rval;
	}
	else
	  return rval;
}

/*******************************************************************
* Connect
*
* Used to establish a connection using a socket
*
* Arguments:
*
*    s       - the fd of the socket to use to connect
*    name    - structure containing info about the socket
*    namelen - length of structure in bytes
*
*******************************************************************/

int Connect(int s, const struct sockaddr * name, int namelen)
{
	int rval; // return value from connect if it succeeds
	rval = connect(s, name, namelen);
	if( rval < 0 ) {
		Close(s);
		PError(CONNECT_FAILED);
		if(exit_flags[CONNECT]) exit(1); 
	}
	return rval;
}

/*******************************************************************
* Close
*
* Closes a file descriptor.
*
* Arguments:
*     filedes - the file descriptor of the file to be closed
*
*******************************************************************/

int Close(int filedes)
{
    int rval; // return value from close
    rval = close(filedes);
    if ( rval < 0 ) {
	    PError(CLOSE_FAILED);
	    if(exit_flags[FPUTS]) exit(1);
	} 
	return rval;
}

/*******************************************************************
* Bind
*
* Binds a name to a socket. After calling,
* the name pointed to by "name" is bound to the socket, which is
* specified by argument "s".
*
* Arguments:
*     s       - a file descriptor to be used in binding the socket
*     name    - the name for the socket
*     namelen - the length of the name being used
*
*******************************************************************/

int Bind(int s, const struct sockaddr * name, int namelen)
{
	int rval;
	rval = bind(s, name, namelen);
	if ( rval < 0 ) {
		Close(s);
		PError(BIND_FAILED);
		if(exit_flags[BIND]) exit(1);
	} 
	return rval;
}

/*******************************************************************
* Listen
*
* Converts an unconnected socket FD to a passive listening
* socket. (moves socket from CLOSED to LISTEN state)
*
* returns 0 if call was successful,
* otherwise exits 
*
* Arguments:
*     sockfd  - file descriptor of unconnected socket
*     backlog - 
*
*******************************************************************/

int Listen(int sockfd, int backlog)
{
	int rval;
	/* create the listening socket */
	rval = listen(sockfd, backlog);
	if( rval  < 0 ) {
		Close(sockfd);  
		PError(LISTEN_FAILED);
		if(exit_flags[LISTEN]) exit(1);
	}
	return rval;
}

/*******************************************************************
* Accept
*
* Accepts a connection on a socket. Creates a new fd for the socket
* which is used to manage the connection.
*
* returns a non-negative integer if successful
* otherwise displays a message and terminates the program
*
* Arguments:
*     s       - a socket created with a call to socket and
*               bound to an address with bind
*
*     addr    - filled in with the address of the entity connecting
*               on the socket 
*
*     addrlen - initially contains length of structure for addr.
*               on exit this is the length of the address put into
*               addr.
*
*******************************************************************/

int Accept(int s, struct sockaddr * addr, socklen_t * addrlen)
{
	int rval; /* return value from accept call */
	
	/* accept the connection */
	acceptagain:
	rval = accept(s, addr, addrlen);
	
	/* if interrupted, try again and again */
	if (errno == EINTR) {
		errno = 0;
		goto acceptagain;
	}
	if(rval < 0) {
		Close(s);
		PError(ACCEPT_FAILED);
		if(exit_flags[ACCEPT]) exit(1);
	} 
	return rval;
}

/*******************************************************************
* Fgets
*
* Wrapper for fgets. Reads characters from "stream" until either
* (n-1) characters have been read, a newline character is read
* and transferred, or the EOF condition occurs.
*
* Arguments:
*     path    - path to directory entry to be removed
*
*******************************************************************/

char *Fgets(char *s, int n, FILE *stream)
{
	char * rval; /* return value from fgets */
	rval = fgets(s, n, stream);
	if( rval == NULL && errno ) {
		PError(FGETS_FAILED);
		if(exit_flags[FGETS]) exit(1);
	}
	return rval;
}

/*******************************************************************
* Writen
*
* Wrapper for writen from in_out.c. Writes the number of characters
* specified by n from the buffer pointed to by vptr to the file
* descriptor indicated by fd.
*
* 
*
* Arguments:
*     fd    - file descriptor to write to
*     vptr  - buffer to copy from
*     n     - the number of bytes to copy
*
*******************************************************************/

ssize_t Writen(int fd, const void * vptr, size_t n)
{
	ssize_t rval;
	rval = writen(fd, vptr, n);
	if (rval < 0) {
		PError(WRITEN_FAILED);
		if(exit_flags[WRITEN]) exit(1);
	}
	return rval;
}

/*******************************************************************
* Readline
*
* Wrapper function for readline from in_out.c.
* Reads up to maxlen 

* Arguments:
*     fd    - file descriptor to write to
*     vptr  - buffer to copy from
*     n     - the number of bytes to copy
*
*******************************************************************/

ssize_t Readline(int fd, void * vptr, size_t maxlen)
{
	ssize_t rval;
	rval = readline(fd, vptr, maxlen);
	if (rval < 0) {
		PError(READLINE_FAILED);
		if(exit_flags[READLINE]) exit(1);
	}
	return rval;
}

/*******************************************************************
* Fork
*
* Wrapper function for fork().

* Arguments:
*     fd    - file descriptor to write to
*     vptr  - buffer to copy from
*     n     - the number of bytes to copy
*
*******************************************************************/

pid_t Fork(void)
{
	pid_t rval;
	rval = fork();
	if(rval == ((pid_t)-1)) {
		PError(FORK_FAILED);
		if(exit_flags[FORK]) exit(1);
	}
	return rval;
}

/*********************************************************************
* Signal
*
* Wrapper function for signal function.

* Arguments:
*     signo    - the name of a signal 
*     func     - either a pointer to a signal handling function
*                or one of the default constants SIG_IGN or SIG_DFL
*
* Return values:
*   On success, returns the signal's "previous disposition".
*   On failure signal returns SIG_ERR, and Signal will invoke
*    PError and exit.
*
*********************************************************************/

Sigfunc * Signal(int signo, Sigfunc * func)
{
	Sigfunc * rval;
	rval = signal(signo, func);
	if (rval == SIG_ERR) {
		PError(SIGNAL_FAILED);
		if(exit_flags[SIGNAL]) exit(1);
	}
	return rval;
}


/*********************************************************************
* Signal
*
* Wrapper function for chdir() function.

* Arguments:
*     pathname    - the pathname of directory to change to 
*
* Return values:
*   On success, returns 0.
*   On failure returns -1 or if exit flag is set, terminates the
*   program.
*
*********************************************************************/

int Chdir(const char * pathname)
{
	int rval;
	/* repeat chdir in case of interruption */
	chdiragain:
	rval = chdir(pathname);
	if( errno == EINTR ) {
		errno = 0;
		goto chdiragain;
	}
	if(rval < 0) {
		PError(pathname);
		if(exit_flags[CHDIR]) exit(1);
	}
	return rval;
}

/*********************************************************************
* Getcwd
*
* Description:
* Wrapper for the getcwd function. Returns a pointer to a buffer
* containing the path of the current working directory.
*
* Arguments:
*     buf    - a string buffer to store the path to the CWD
*     size   - the size of the buffer for holding the CWD
*
* Return values:
* On success, returns a pointer to a buffer containing the path of
*   the current working directory.
* On failure, returns null.
*
*********************************************************************/
char * Getcwd(char * buf, size_t size)
{
	char * rval;
	rval = getcwd(buf, size);
	if(!rval) {
		PError(GETCWD_FAILED);
		if(exit_flags[GETCWD]) exit(1);
	}
	return rval;
}


/*********************************************************************
* Getenv
*
* Description:
* Wrapper for the getenv function. Searches the environment list
* for a string of the form name=value and if the string is present
* returns a pointer to that value.
*
* Arguments:
*     name    - the name of the variable whose value is being 
*               searched for
*
* Return values:
* On success, returns a pointer to the value in the current
* environment.
* On failure, returns null.
*
*********************************************************************/

char * Getenv(const char * name)
{
	char * rval;
	rval = getenv(name);
	if(!rval) {
		PError(GETENV_FAILED);
		if(exit_flags[GETENV]) exit(1);
	}
	return rval;
}
