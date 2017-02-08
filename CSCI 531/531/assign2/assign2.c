/*******************************************************************
* Brandon Tweed
* CSCI 531
* Instructor: Onyuksel
* Homework Assignment 2
* Date: 2-15-2005
*
* Description:
* This program is a client that connects to a simple daytime
* server, gets the date, and then sends the result to the
* standard output.
*
*******************************************************************/

#include"assign2.h"

int main(int argc, char ** argv)
{
	
  int sockfd, n;               // fd for the socket, num chars read
  char recvline[MAXLINE+1];    // buffer used by the read
  struct sockaddr_in servaddr; // holds information about socket 
  
  /* if user didn't enter an IP, quit */
  if (argc != 2) {
      fprintf(stderr,"Usage: %s <IPaddress>\n", argv[0]);
      exit(1);
  }
  
  /* create the socket */
  sockfd = Socket(AF_INET, SOCK_STREAM, 0);
  
  /* clear the servaddr structure */
  bzero(&servaddr, sizeof(servaddr));
  
  /* specify IPv4 and port number for the connection */
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERVER_PORT);
  
  /* convert text IP to something network can understand */
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  
  /* now establish connection using the socket we've created */
  Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

  /* read all the data from the socket and write to
     the standard output */
  while( (n = Read(sockfd, recvline, MAXLINE)) > 0) {
  	recvline[n] = 0;
    Fputs(recvline,stdout);
  }
  
  /* close the socket */
  Close(sockfd);
  
  exit(0);
}
