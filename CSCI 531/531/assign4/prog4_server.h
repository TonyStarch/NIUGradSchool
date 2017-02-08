/**************************************************************
* prog4_server.h
* Author: Brandon Tweed
* Course: CSCI 531
* Date:   3/28/2005
*
* Description: 
* This header file contains prototypes and constants used 
* by the server implemented in prog4_server.c
*
**************************************************************/

#ifndef  PROG4_SERVER_H
#define  PROG4_SERVER_H

#include"wrapper.h"
#include"in_out.h"

#define UNKNOWN_MSG "Unknown"  /* message returned when not found */

/* the path to the database file */
#define DB_FILE "/usr2/onyuksel/courses/531/progs/05s/p4/prog4.d"  

#define MY_SERV_PORT 6146      /* port that server uses for connect */

/********************************************************************
* prototypes
********************************************************************/

/* handles child's duty of responding to the client */
void str_respond(int fd);

/* converts a string to all lower case */
void makelower(char * s);

/* searches the database for the matching title */
void search_db(FILE * db, char * author, char * title);

/* signal function for handling signals from children */
void sig_chld(int signo);

#endif
