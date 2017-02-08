/********************************************************************
* Brandon Tweed
* Assignment 3
* CSCI 531
* Instructor: Onyuksel
* Date: 2/28/2005
*
* Description:
* This file contains the main function which implements the
* server for assignment 3.
*
********************************************************************/

#include"prog3_server.h"

int main(int argc, char ** argv) /* argc, argv unused */
{
  int listenfd, connfd;     /* fds for listen and connect sockets */
  int n;                    /* number of bytes read from socket */
  struct sockaddr servaddr; /* holds info about the domain socket */
  
  char buff[MAXLINE];       /* output buffer */
  char messg[MAXLINE];      /* buffer for holding message to print */
  
  /* create the socket */
  listenfd = Socket(AF_UNIX, SOCK_STREAM, 0);
  
  /* clear the servaddr structure */
  bzero(&servaddr, sizeof(servaddr));
  
  /* set attributes for the socket */
  servaddr.sa_family = AF_UNIX;          /* UNIX domain socket */
  strcpy(servaddr.sa_data, SOCKET_NAME); /* set socket name */
  
  /* bind to create the name for the socket */
  Bind(listenfd,(struct sockaddr *) &servaddr, sizeof(servaddr));
  
  /* convert socket to a listening socket */
  Listen(listenfd, LISTENQ);
  
  /* wait for data sent by the client */
  connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL);
  
  /* read the incoming data from the client */
  n = Read(connfd, buff, MAXLINE);
  
  /* copy the data to a formatted string */
  snprintf(messg, n, "%s", buff); 
  
  /* print output messages to stdout */
  fprintf(stdout,"Server received \"%s\".",messg);
  fprintf(stdout,"\nIt is %d bytes long.\n", n);
  
  /* close the connection */
  Close(connfd);
  Close(listenfd);
  
  /* unlink the directory entry created by Bind */
  Unlink(servaddr.sa_data);
  
  return 0;
}
