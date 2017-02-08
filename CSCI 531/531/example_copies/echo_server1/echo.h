#include "/usr2/onyuksel/courses/531/common/531.h"

#ifndef H_ECHO
#define H_ECHO

#define NCLNTS 5
#define SLEEP_TIME 30

void str_cli(FILE*, int);
void str_echo(int);
void sig_chld(int);

int Accept(int, SA*, socklen_t*);
int Bind(int, const SA*, int);
int Close(int);
int Connect(int, const SA*, int);
char* Fgets(char*, int, FILE*);
pid_t Fork();
int Fputs(const char*, FILE*);
int Inet_pton(int, const char*, void*);
int Listen(int, int);
int Socket(int, int, int);

ssize_t Readline(int, void*, size_t);
ssize_t Writen(int, const void*, size_t);

Sigfunc* Signal(int, Sigfunc*);
pid_t Wait(int*);

#endif

