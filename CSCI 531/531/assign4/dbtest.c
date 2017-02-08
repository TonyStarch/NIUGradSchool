
#include "/usr2/onyuksel/courses/531/common/531.h"

#define FOPEN_ERROR "Error: failure to open file"
#define UNKNOWN_MSG "Unknown"
void search_db(FILE * db, char * author, char * title);
void makelower(char * s);

int main(void) 
{
  FILE * db;
  int n;
  char sometitle[30];
  char result[30];
  db = fopen("prog4.d","r");
  if(!db) {
 	fprintf(stderr,FOPEN_ERROR), exit(1);
  }
  printf("Enter a title of a movie: ");
  
  while(fgets(sometitle, 30, stdin)) {
		n = strlen(sometitle);
		sometitle[n-1] = '\0';
		search_db(db, result, sometitle);
		printf("The matching author was: %s\n", result);
		printf("Enter a title of a movie: ");
  }
  
  fclose(db);	
  return 0;
}

/********************************************************************
* search_db
*
* Description:
* Searches the database for the author matching the title, given
* by author and title as arguments.
*
********************************************************************/
void search_db(FILE * db, char * author, char * title)
{
	int found, n;
	char t1[MAXLINE],t2[MAXLINE];
  char inpline[MAXLINE];
	found = 0;	
	
	makelower(title);
	printf("The title being searche for: ***%s***\n", title);

	/* do the search */
	while(fgets(inpline, MAXLINE, db)) {
		sscanf(inpline,"%[^:]:%[^:];",t1,t2);
		makelower(t1);
		if(strcmp(t1,title) == 0){ 
			found = 1;
			break;
		}
	}
	
	if (!found) {
		n = strlen(UNKNOWN_MSG);
		snprintf(author, n+1, "%s", UNKNOWN_MSG);
	} else {
		n = strlen(t2);
		snprintf(author, n+1, "%s", t2);
	}
	
	/* reset the file pointer to beginning of file */
	fseek(db, 0, SEEK_SET);
}

void makelower(char * s) 
{
	char * ptr;
	ptr = s;
	while( *ptr != '\0' ) {
		*ptr = tolower(*ptr);
		ptr++;
	}
		
}
