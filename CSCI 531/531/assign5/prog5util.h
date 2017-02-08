/*******************************************************************
* Brandon Tweed        *  prog5util.h *
* CSCI 531             ****************
* Assignment 5
* 4/25/2005
*
* Description:
* This file contains prototypes of functions, constants, and
* enumerated types used by prog5client.c, prog5server.c, and 
* FTP.c.
*
********************************************************************/

#ifndef PROG5_UTIL
#define PROG5_UTIL

#include"wrapper.h"

typedef enum { CMD_BLANK, CMD_BYE, CMD_CD, CMD_CDUP,
				CMD_CLOSE, CMD_DELETE, CMD_HELP, CMD_LCD, CMD_MKDIR, 
				CMD_OPEN, CMD_PWD, CMD_RENAME, CMD_RMDIR, 
				CMD_DISCONNECT, CMD_QM, CMD_QUIT, CMD_ERROR } cmd_t;

/* prompt used by the FTP program */
#define FTP_PROMPT     "ftp> "

/* response when an invalid command is typed */
#define INVALID_CMD    "?Invalid command\n"

/* name of environment variable containing path to home directory */
#define HOME_VARIABLE  "HOME"

/********************************************
* The following are used by the help command
********************************************/

#define NCMDS    16  /* number of commands total for help */
#define NCOLS    3   /* number of columns to print of commands */

/* messages for prompting the user for input */
#define MKDIR_PROMPT      "(directory-name) "
#define RMDIR_PROMPT      MKDIR_PROMPT
#define CD_PROMPT         "(remote directory) "
#define DELETE_PROMPT     "(remote file) "
#define RENAME_PROMPT1    "(from-name) "
#define RENAME_PROMPT2    "(to-name) "
#define OPEN_PROMPT       "(to) " 

/* error messages for user prompting */
#define MKDIR_ERROR       "usage: mkdir directory-name\n"
#define CD_ERROR          "usage: cd remote-directory\n"
#define DELETE_ERROR      "usage: delete remote-file\n"
#define RENAME_ERROR      "rename from-name to-name\n"
#define RMDIR_ERROR       "usage: rmdir directory-name\n"
#define OPEN_ERROR        "usage: open ip-address\n"


/*********************************************************************
* prototypes 
*********************************************************************/

/* checks which command the user typed in and returns an int */
cmd_t check_cmd(char * incmd);

/* takes care of printing help information */
void do_help(char * arg, int numargs);

/* takes care of the local cd command */
void do_LCD(char * arg, int numargs);

/* substitutes value of the home variable for paths that begin
   with a tilde */
int interpolate_tilde(char * dest, int maxdest, const char * src);

/* removes a newline from a string given by the user */
void remove_newline(char * buf);

#endif
