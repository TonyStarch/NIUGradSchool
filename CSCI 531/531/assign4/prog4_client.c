/********************************************************************
*
* File:       prog4_client.c
* Author:     Brandon Tweed
* Date:       3/28/2005
* Course:     CSCI 531
* Instructor: Ibrahim Onyuksel
*
* Description:
* This file implements the client program portion of assignment 4.
*
* This program accepts only one command line argument which is
* the IP of the server to connect to. After establishing the 
* connection, the client repeatedly 
*  1) asks the user to enter a title 
*  2) transmits the title to the server
*  3) receives the response from the server
*  4) prints the response on stdout for the user
* until the user causes an EOF condition by typing Ctrl-D.
*
* Note:
*   * The title sent by the user contains a newline character so
*     that the server's call to readline will succeed.
*
*   * The author received from the server will also contain a 
*     newline so that this client's call to readline will complete
*     properly.
*
********************************************************************/

#include"prog4_client.h"

int main(int argc, char ** argv)
{
  int sockfd;
  struct sockaddr_in servaddr;
  
  /* verify that an IP was given */
  if( argc != 2) {
          fprintf(stderr, "%s\n", strerror(157));
	  fprintf(stderr, "%s requires an IP for the server\n", argv[0]);
	  fprintf(stderr, "Usage: %s 127.0.0.1\n", argv[0]);
	  exit(1);
  }

  /* create the socket */
  sockfd = Socket(AF_INET, SOCK_STREAM, 0);
  
  /* set up the servaddr structure */
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(MY_SERV_PORT);
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  
  /* establish the connection actively */
  Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

  /* read the title of a book from stdin, send, and get result */   
  str_cli(stdin, sockfd);
 
  /* close the socket and exit normally */
  Close(sockfd);
  exit(0);
}

/*********************************************************************
* str_cli
*
* Performs the primary job of the client. Reads the title from
* fp (should be stdin), sends it to the server, receives the
* server's response and then prints it out.
*
* Description:
* 1) reads the title of a book from stdin
* 2) sends the title to the server to obtain the author of the book
* 3) prints the obtained author on stdout
*
* Variables:
*
*  n         - the length of the title string sent by the client
*  recvline  - buffer containing line received from server
*  sendline  - buffer containing line to be sent to server
*
*********************************************************************/

void str_cli(FILE * fp, int sockfd)
{
	int n;
	char recvline[MAXLINE], sendline[MAXLINE];
	
	/* ask user for the book's title */
	fprintf(stdout, "Title of book : ");
	
	/* repeat as long as user entered something */
	while( Fgets(sendline, MAXLINE, fp) != NULL ) {
	
		n = strlen(sendline);
				
		/* send the title */
		Writen(sockfd, sendline, n);
		
		/* get the result and print*/
		n = Readline(sockfd, recvline, MAXLINE);
		fprintf(stdout,"Author of book: ");
		Fputs(recvline,stdout);
		
		fprintf(stdout,"Title of book : ");
	}
}
