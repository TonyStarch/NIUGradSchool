/*********************************************************************
* Brandon Tweed
* CSCI 531
* Homework 2
*
* Description:
* This file contains the implementations for the wrapper functions
* defined in wrapper.h. These are used by the main program for 
* opening the socket, getting data from the socket, etc.
*
*********************************************************************/

/*********************************************************************
* wrapper.c
*
* description: 
* File containing wrapper functions used for UNIX system calls
*
*********************************************************************/

// Bring in prototypes for wrapper functions
#include"wrapper.h"

/******************************************************************
* PError
*
* Wrapper for the system perror() function. Accepts a string
* as an argument
*
* arguments:
*     s - the string to print as first part of the error message
*
* variables:
*     errno (external) - reset to zero to indicate no errors
*
******************************************************************/
void PError(const char *s)
{
  // call perror()using the string sent in as an arg
  perror(s);
  /* clear the stream */
  fflush(stderr);
  // reset errno
  errno = 0;
  return;
}

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
		PError("inet_pton error");
		exit(1);
	}
	else if( rval == 0 ){
		fprintf(stderr,"inet_pton error: Invalid address\n");
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
		PError(CONNECT_FAILED); exit(1);
	}
	else
	  return rval;
}

/*******************************************************************
* Read
*
* used to read data from a file (the socket in this case)
*
* Arguments:
*    filedes - file descriptor of socket to read from
*    buf     - buffer to put incoming data into
*    nbyte   - the maximum number of bytes for the read buffer
*
*******************************************************************/
ssize_t Read(int filedes, void * buf, ssize_t nbyte)
{
	int n;  // returned number of bytes read
	n = read(filedes, buf, nbyte);
	if ( n < 0 ) {
		PError("read error"); exit(1);
	} else
		return n;
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
