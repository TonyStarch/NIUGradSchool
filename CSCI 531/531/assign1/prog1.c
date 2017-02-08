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

/********************************************************************
* prog1.c
*
* Description:
* Main program that uses prog01.h and wrapper functions from
* wrapper.c to perform a directory listing
*
********************************************************************/

/* include prog1 header file with prototypes */
#include"prog1.h"

int main(int argc, char * argv[]) 
{
    int i;

    if (argc > 2) {
      
      /* if arguments supplied, cycle through and print listings
         for each */
           
      i = 1;
    
    	while( i < argc) {
	    		printf("\ndirectory: %s\n",argv[i]);
	    		fflush(stdout);
    			print_dir(argv[i]);
    			i++;
			}
				
    } else {
    	
    	/* otherwise give a listing of the current directory */
    	
    	printf("\ndirectory: .\n");
    	fflush(stdout);
    	print_dir(".");
      
    }
    
	return 0; 
}


/********************************************************************
* print_dir
*
* description:
* prints the listing of the directory, which is specified as a 
* string
*
* arguments:
*     dirarg - char * argument that points to a string. the string
*              represents the path of the directory to list
*
* variables:
*     dp   - a pointer to the directory created by opendir
*     dirp - a pointer to a structure used to hold information
*            about the directory being read
********************************************************************/

void print_dir(const char * dirarg) 
{
	
	DIR* dp;
  struct dirent* dirp;
  
	/*open the directory */
	dp = OpenDir(dirarg);
	
	/* if call to OpenDir successful, read and print */
	if(dp) {
      while ((dirp = ReadDir(dp)))
      		printf("%s\n", dirp->d_name);
      		/* make sure contents are immediately written out */
      		fflush(stdout);
      		CloseDir(dp);
  }
  
}
