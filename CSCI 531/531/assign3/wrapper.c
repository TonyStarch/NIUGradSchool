/*********************************************************************
* Brandon Tweed
* CSCI 531
* Homework 3
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
	if( !rval )
		return rval;
	else if(rval == -1) {
		PError(LISTEN_FAILED); exit(1);
	}	
	/* shouldn't make it here */
	PError("Return value from listen not 0 or -1");
	exit(1);
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
	rval = accept(s, addr, addrlen);
	if(rval < 0) {
		PError(ACCEPT_FAILED); exit(1);
	} else
		return rval;
}

/*******************************************************************
* Write
*
* Used to write data to a socket.
*
* returns a non-negative integer if successful
* otherwise displays a message and terminates the program
*
* Arguments:
*     filedes - file descriptor to be written to
*
*     buf     - buffer to copy data to socket from 
*
*     nbytes  - number of bytes to copy to socket from buffer
*
*******************************************************************/
ssize_t Write(int filedes, const void * buf, size_t nbyte)
{
	int n; /* number of bytes written */
	/* write the data */
	n = write(filedes, buf, nbyte);
	if( n < 0 ) {
		PError(WRITE_FAILED); exit(1);
	} else
		return n;
}

/*******************************************************************
* Unlink
*
* Used to unlink an entry that's created by opening a UNIX
* domain socket.
*
* Arguments:
*     path    - path to directory entry to be removed
*
*******************************************************************/
int Unlink(const char * path)
{
	int rval;
	rval = unlink(path);
	if(rval < 0) {
		PError("Error unlinking"); exit(1);
	} else
		return rval;
	
}
