/**************************************************************
* wrapper.h
* Brandon Tweed
* CSCI 531
**************************************************************/

#ifndef WRAPPER_H
#define WRAPPER_H


/* Header file created by Prof Onyuksel which includes
   various header files required for system calls that
   are a part of the Sockets API. Equivalent to unp.h
   in the text book */
#include "/usr2/onyuksel/courses/531/common/531.h"

/**************************************************************
* constants for error messages
**************************************************************/

#define CONNECT_FAILED  "Error connecting"
#define SOCKET_FAILED   "Error opening socket"
#define CLOSE_FAILED    "Error closing socket"
#define FPUTS_FAILED    "fputs error"
#define BIND_FAILED     "Error binding socket"
#define LISTEN_FAILED   "Error creating listening socket"
#define ACCEPT_FAILED   "Error accepting connection on socket"
#define WRITE_FAILED    "Error writing to socket"

/*************************************************************
* prototypes
*************************************************************/

/*displays an error message and resets errno */
void PError(const char * s);

/* for opening a communication socket, returns an fd */
int Socket(int domain, int type, int protocol);

/* for converting a string address to a presentation address */
int Inet_pton(int, const char *, void *);

/* for connecting to a server using a socket */
int Connect(int, const struct sockaddr *, int);

/* for reading information from the socket */
ssize_t Read(int, void *, ssize_t);

/* for writing a string to a file */
int Fputs(const char * s, FILE *stream);

/* for closing a file descriptor */
int Close(int filedes);

/* for converting a listen fd to a connected socket */
int Bind(int, const struct sockaddr *, int);

/* for converting an unconnected socket to a passive listening
   socket */
int Listen(int, int);

/* for accepting a connection on a listening socket */
int Accept(int, struct sockaddr *, socklen_t *);

/* for writing data to a socket */
ssize_t Write(int, const void *, size_t);

/* for removing the directory entry created by a domain socket */
int Unlink(const char * path);

#endif
