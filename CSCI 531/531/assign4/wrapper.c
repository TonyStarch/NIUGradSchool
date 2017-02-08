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
   if( sockfd < 0 )
       PError(SOCKET_FAILED),exit(1);
   else
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
		exit(1);
	}
	else if( rval == 0 ){
		fprintf(stderr,INET_PTON_INVALID);
		exit(1);
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
		PError(CONNECT_FAILED), exit(1); 
	}
	else
	  return rval;
}

/*******************************************************************
* Fputs
*
* Writes a given string to a file specified using a file pointer.
*
* Arguments:
*    s      - the string to write out
*    stream - the file to write the string to
*
*******************************************************************/

int Fputs(const char * s, FILE *stream)
{
	int n; // returned number of bytes written or EOF
	if ((n = fputs(s,stream)) == EOF) {
    	PError(FPUTS_FAILED);
    	exit(1);
    } else
        return n;
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
	    PError(CLOSE_FAILED); exit(1);
	} else
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
		PError(BIND_FAILED); exit(1);
	} else
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
		PError(LISTEN_FAILED), exit(1);
	} else 
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
		PError(ACCEPT_FAILED), exit(1);
	} else
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
		PError(FGETS_FAILED), exit(1);
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
		PError(WRITEN_FAILED), exit(1);
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
		PError(READLINE_FAILED), exit(1);
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
		PError(FORK_FAILED), exit(1);
	}
	return rval;
}

/*******************************************************************
* Fclose
*
* Wrapper function for fclose().

* Arguments:
*     stream - file pointer to file to close
*
* Return values:
*   On success, returns 0
*   On failure (EOF) returned from fclose, calls PError and exits.
*
*******************************************************************/

int Fclose(FILE * stream)
{
	int rval;
	rval = fclose(stream);
	if(rval == EOF) 
		PError(FCLOSE_FAILED), exit(1);
	return rval;
}

/*******************************************************************
* Fopen
*
* Wrapper function for fopen().

* Arguments:
*     filename - a string indicating the file name of the file
*                to open
*     mode     - a string indicating what mode the file being opened
*                is to be manipulated with. Ex. read, write, etc.
*
* Return values:
*   On success, returns a pointer to an object controlling the stream.
*   On failure fopen returns a null pointer. Fopen calls PError and 
*   exits.
*
*******************************************************************/

FILE * Fopen(const char * filename, const char * mode)
{
	FILE * rval;
	rval = fopen(filename, mode);
	if(!rval) {
		PError(FOPEN_FAILED), exit(1);
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
		PError(SIGNAL_FAILED), exit(1);
	}
	return rval;
}

