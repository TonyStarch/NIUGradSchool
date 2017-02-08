/*********************************************************************
* Brandon Tweed
* CSCI 531
* Homework 1
* Due 2-2-2005
*
* Description: This file is part of Homework 1, a program that is
* designed to produce a directory listing.
*
*********************************************************************/

/*********************************************************************
* wrapper.c
*
* description: 
* File containing wrapper functions used for UNIX system calls
*
*********************************************************************/

// Bring in prototypes for wrapper functions
#include"wrapper.h"

/******************************************************************
* PError
*
* Wrapper for the system perror() function. Accepts a string
* as an argument
*
* arguments:
*     s - the string to print as first part of the error message
*
* variables:
*     errno (external) - reset to zero to indicate no errors
*
******************************************************************/
void PError(const char *s)
{
  // call perror()using the string sent in as an arg
  perror(s);
  /* clear the stream */
  fflush(stderr);
  // reset errno
  errno = 0;
  return;
}

/*****************************************************************
* OpenDir
*
* Wrapper for opendir() system call. Accepts a string as an 
* argument.
*
* arguments:
*     dirname - pointer to a string that represents the path
*               of the directory to list
*
* variables:
*     dp - a directory pointer used by calls to the system
*
* return value:
*     On successful open, a valid directory pointer.
*     On failure, a NULL pointer.
*
*****************************************************************/
DIR * OpenDir(const char *dirname)
{ 
	DIR * dp;
	dp = opendir(dirname);
	// check if result was null
	if(!dp) {
		perror(OPENDIR_FAILED);
	}
	return dp;
}

/*****************************************************************
* ReadDir
*
* Wrapper for readdir() system call. Accepts a directory pointer
* as an argument.
*
* arguments:
*     dp - pointer to an open directory; used by UNIX system call
*
* variables:
*     pdirent - a directory entry pointer returned by the
*               readdir system call
*
* return value:
*     On successful read, a valid directory entry pointer.
*     On failure, a NULL pointer.
*****************************************************************/
struct dirent * ReadDir(DIR *dp)
{
  struct dirent * pdirent;
  pdirent = readdir(dp);
  if(!dp){
  	PError(READDIR_FAILED);
  }
  return pdirent;
}

/*****************************************************************
* CloseDir
*
* Wrapper for closedir() system call. Accepts a directory pointer
* as an argument.
*
* arguments:
*     dp - a directory pointer indicating the directory to close
*
* variables:
*     rval - integer to hold return value from closedir system
*            call
*
* return value:
*     On successful close, 0.
*     On failure, -1.
*
*****************************************************************/
int CloseDir(DIR *dp)
{
	int rval;
	rval = closedir(dp);
	if(rval == 0) {
		return 0;
	} else {
		PError(CLOSEDIR_FAILED);
		return -1;
	}
}
