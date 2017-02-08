/**************************************************************
* in_out.h
* Author: Brandon Tweed
* Course: CSCI 531
* Date:   3/28/2005
*
* Description: 
* This header file contains prototypes and constants used 
* by the I/O routines defined in in_out.c
*
**************************************************************/

#ifndef  IN_OUT_H
#define  IN_OUT_H

/* Prof. Onyuksel's header file */
#include "/usr2/onyuksel/courses/531/common/531.h"

/********************************************************************
* prototypes
********************************************************************/
/* for writing n bytes to a file descriptor */
ssize_t writen(int fd, const void * vptr, size_t n);

/* for reading up to maxlen bytes from a fd */
ssize_t readline(int fd, void * vptr, size_t maxlen);

#endif
