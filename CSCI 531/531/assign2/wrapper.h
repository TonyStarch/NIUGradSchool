/*********************************************************************
* Brandon Tweed
* CSCI 531
* Homework 2
* 2-15-2005
*
* Description: 
* This wrapper file contains constants and function prototypes for
* the functions that are used as part of assignment 2. These 
* functions perform operations like creating a socket, establishing
* a connection, and so on.
*
*********************************************************************/

/*********************************************************************
* wrapper.h
*
* description:
* This is the header file for wrapper.c, a file which holds wrapper
* functions for a variety of UNIX system calls. Included are the
* prototypes for the wrapper functions, defined constants for 
* error messages, and includes that bring in source for the system
* calls
*
********************************************************************/

#ifndef WRAPPER_H
#define WRAPPER_H
 
/* Following is header file created by Prof Onyuksel
   which includes various header files required for system calls
   that are part of the Sockets API */S   
#include"/usr2/onyuksel/courses/531/common/531.h"


/***************************************************
* defined constants
***************************************************/

/* these are error messages used by PError() */
#define CONNECT_FAILED  "Error connecting"
#define SOCKET_FAILED   "Error opening socket"
#define CLOSE_FAILED    "Error closing socket"
#define FPUTS_FAILED    "fputs error"
#define BIND_FAILED     "Error binding socket"

/*****************************
* prototypes
*****************************/

/* displays an error message and resets errno */
void PError(const char *s);

/* for opening a communication socket , returns a fd */
int Socket(int domain, int type, int protocol);

/* for converting a string address to a presentation address */
int Inet_pton(int af, const char * cp, void * addr);

/* for connecting to the server using a socket */
int Connect(int s, const struct sockaddr * name, int namelen);

/* for reading information from the socket */
ssize_t Read(int filedes, void * buf, ssize_t nbyte);

/* for writing a string to a file */
int Fputs(const char * s, FILE *stream);

/* for closing a file descriptor */
int Close(int filedes);

/* for converting a listenfd to a connected socket */
int Bind(int s, const struct sockaddr * name, int namelen);

#endif
