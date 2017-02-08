#include "/usr2/onyuksel/courses/531/common/531.h"
#include "/usr2/onyuksel/courses/531/common/wrapper.h"

#ifndef H_ECHO
#define H_ECHO

#define NCLNTS 5

extern bool exit_flags[];

void str_cli(FILE*, int);
void str_echo(int);
void sig_chld(int);
void init_flags();

int Accept(int, SA*, socklen_t*);
int Bind(int, const SA*, int);
int Close(int);
int Connect(int, const SA*, int);
int Inet_pton(int, const char*, void*);
int Listen(int, int);
int Socket(int, int, int);

char* Fgets(char*, int, FILE*);
int Fputs(const char*, FILE*);

pid_t Fork();
Sigfunc* Signal(int, Sigfunc*);
pid_t Wait(int*);

ssize_t Readline(int, void*, size_t);
ssize_t Readn(int, void*, size_t);
ssize_t Writen(int, const void*, size_t);

#endif

