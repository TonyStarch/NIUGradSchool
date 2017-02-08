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
* wrapper.h
*
* description:
* This is the header file for wrapper.c, a file which holds wrapper
* functions for a variety of UNIX system calls. Included are the
* prototypes for the wrapper functions, defined constants for 
* error messages, and includes that bring in source for the system
* calls
*
********************************************************************/

#ifndef WRAPPER_H
#define WRAPPER_H
 
// Following is header file created by Prof Onyuksel
#include"/usr2/onyuksel/courses/531/common/531.h"

// This is system code for processing directory listings
#include<dirent.h>

// This is a header with information about various system types
#include<sys/types.h>

// This is for using perror() to print error messages
#include<errno.h>

/***************************************************
* defined constants
***************************************************/

/* these are error messages used by PError() */
#define OPENDIR_FAILED  "Error opening file"
#define READDIR_FAILED  "Error reading directory"
#define CLOSEDIR_FAILED "Error closing directory"

/*****************************
* prototypes
*****************************/

void PError(const char *s);
DIR * OpenDir(const char *dirname);
int CloseDir(DIR *dirp);
struct dirent * ReadDir(DIR *dirp);

#endif
