/**************************************************************
* prog5server.h
* Author: Brandon Tweed
* Course: CSCI 531
* Date:   4/25/2005
*
* Description: 
* This header file contains prototypes and constants used 
* by the server implemented in prog5server.c
*
**************************************************************/

#ifndef  PROG5_SERVER_H
#define  PROG5_SERVER_H

#include"prog5util.h"

/*********************************************************************
* constants
*********************************************************************/

/* port used to connect to server */
#define MY_SERV_PORT 5115

/*********************************************************************
* prototypes  
*********************************************************************/

/* handles the child's responsibility of accepting FTP commands */
void ftp_cmd(int connfd);

/* handles the signal for termination of a child */
void sig_chld(int signo);

/* for performing the cdup command */
void serv_cdup(int fd);

/* for performing the mkdir command */
void serv_mkdir(int fd, char * arg1);

/* for performing the rmdir command */
void serv_rmdir(int fd, char * arg1);

/* for performing the pwd command */
void serv_pwd(int fd);

/* for performing the cd command */
void serv_cd(int fd, char * arg1);

/* for performing the delete command */
void serv_delete(int fd, char * arg1);

/* send the value of errno back to the client */
void return_errno(int fd);

/* set server's exit flags */
void set_flags(void);

/* rename a file from arg1 to arg2 */
void serv_rename(int fd, char * arg1, char * arg2);

#endif
