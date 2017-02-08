/*******************************************************************
* Brandon Tweed        *  prog5client.h *
* CSCI 531             ******************
* Assignment 5
* 4/25/2005
*
* Description:
* This file contains prototypes for functions and constants used
* by the prog5client portion of the client program.
*
********************************************************************/

#ifndef PROG5_CLIENT_H
#define PROG5_CLIENT_H

#include"prog5util.h"

/*********************************************************************
* constants
*********************************************************************/

/* port number for connecting to the server */
#define MY_SERV_PORT 5115

/*********************************************************************
* prototypes  
*********************************************************************/

/* the main client function that invokes a command interpreter */
int prog5client(char * serverIP, int numargs);

/* gets the errno from the server and returns it as an int */
int get_response(int sock_fd);

/* sends the command to the server with arguments */
void send_cmd(int sockfd, cmd_t cmdval, char *arg1, char *arg2);

/* send pwd to server and handle response */
int cli_pwd(int sockfd);

/* send mkdir to server and handle response */
int cli_mkdir(int sockfd, char * arg, int numargs);

/* send cdup to server and handle response */
int cli_cdup(int sockfd) ;

/* send cd command to server and handle response */
int cli_cd(int sockfd, char * arg, int numargs);

/* send delete command to server */
int cli_delete(int sockfd, char * arg, int numargs);

/* send rename command to server */
int cli_rename(int sockfd, char * arg1, char * arg2, int numargs);

/* send rmdir command to the server */
int cli_rmdir(int sockfd, char * arg, int numargs);

/* prompt user to enter input for a non-entered argument */
int prompt_user(const char * prompt, char * buf, int bufsize, 
				const char * errmsg);

#endif
