/********************************************************************
* Brandon Tweed
* Assignment 3
* CSCI 531
* Instructor: Onyuksel
* Date: 2/28/2005
*
* Description:
* This file implements the client program portion of homework
* assignment 3. The client connects to the server, send the message
* "Hello" and then terminates.
*
********************************************************************/

#include"prog3_client.h"

int main(int argc, char ** argv)
{
  int sockfd,               /* fd used in creating the socket */
      n;                    /* size of greeting message to send */
  char sendln[MAXLINE];     /* output buffer */
  struct sockaddr servaddr; /* holds information about the socket */
  
  /* create the socket */
  sockfd = Socket(AF_UNIX, SOCK_STREAM, 0);
  
  /* clear the servaddr structure */
  bzero(&servaddr, sizeof(servaddr));
  
  servaddr.sa_family = AF_UNIX; /* this is a UNIX domain socket */
  strcpy(servaddr.sa_data, SOCKET_NAME); /* set server name */
  
  /* establish the connection actively */
  Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
  
  /* calc length of string sent including NULL terminating char */
  n = strlen(GREETING_MSG) + 1;
  
  /* format data to be sent in the buffer */
  snprintf(sendln, MAXLINE, "%s", GREETING_MSG);
  
  /* write data to the socket */
  Write(sockfd, sendln, n);
  
  /* close the socket */
  Close(sockfd);
  
  return 0;
}
