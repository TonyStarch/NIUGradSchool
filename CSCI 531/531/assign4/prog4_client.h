/**************************************************************
* prog4_client.h
* Author: Brandon Tweed
* Course: CSCI 531
* Date:   3/28/2005
*
* Description: 
* This header file contains prototypes and constants used 
* by the client implemented in prog4_client.c
*
**************************************************************/
#ifndef  PROG4_CLIENT_H
#define  PROG4_CLIENT_H

#include"wrapper.h"
#include"in_out.h"

#define MY_SERV_PORT 6146 /* port for server to connect to */

/*******************************************************************
* prototypes 
********************************************************************/

/* this function send the title to the server and gets the author */
void str_cli(FILE * fp, int sockfd);

#endif
