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

#define CONNECT_FAILED    "Error connecting"
#define SOCKET_FAILED     "Error opening socket"
#define CLOSE_FAILED      "Error closing socket"
#define FPUTS_FAILED      "fputs error"
#define BIND_FAILED       "Error binding socket"
#define LISTEN_FAILED     "Error creating listening socket"
#define ACCEPT_FAILED     "Error accepting connection on socket"
#define WRITE_FAILED      "Error writing to socket"
#define FGETS_FAILED      "fgets error occurred"
#define WRITEN_FAILED     "writen error occurred"
#define READLINE_FAILED   "readline error occurred"
#define FORK_FAILED       "error forking child"
#define FOPEN_FAILED      "Error: failure to open file"
#define FCLOSE_FAILED     "Error: failure to close file"
#define SIGNAL_FAILED     "error calling signal function"
#define INET_PTON_FAILED  "inet_pton error"
#define INET_PTON_INVALID "inet_pton error: Invalid address\n"

/**************************************************************
* macros
**************************************************************/

/* calls perror and resets the errno variable */
#define PError(s)   perror(s),errno = 0

/*************************************************************
* prototypes
*************************************************************/

/* for opening a communication socket, returns an fd */
int Socket(int domain, int type, int protocol);

/* for converting a string address to a presentation address */
int Inet_pton(int, const char *, void *);

/* for connecting to a server using a socket */
int Connect(int, const struct sockaddr *, int);

/* for closing a file descriptor */
int Close(int filedes);

/* for converting a listen fd to a connected socket */
int Bind(int, const struct sockaddr *, int);

/* for converting an unconnected socket to a passive listening
   socket */
int Listen(int, int);

/* for accepting a connection on a listening socket */
int Accept(int, struct sockaddr *, socklen_t *);

/* for getting a sequence of characters from a file */
char *Fgets(char *s, int n, FILE *stream);

/* for forking to produce a child process */
pid_t Fork(void);

/* for using fclose to close a file */
int Fclose(FILE * stream);

/* for using fopen to open a file */
FILE * Fopen(const char * filename, const char * mode);

/* for writing to a file */
int Fputs(const char * s, FILE *stream);

/* wrapper for the signal function */
Sigfunc * Signal(int signo, Sigfunc * func);

/********************************************************************
* I/O function prototypes
********************************************************************/

/* writes n bytes to a fd */
ssize_t Writen(int fd, const void * vptr, size_t n);

/* reads until maxlen or newline from fd */
ssize_t Readline(int fd, void * vptr, size_t maxlen);

#endif
