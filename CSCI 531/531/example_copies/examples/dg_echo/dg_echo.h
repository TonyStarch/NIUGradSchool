#include "/usr2/onyuksel/courses/531/common/531.h"
#include "/usr2/onyuksel/courses/531/common/wrapper.h"

#ifndef H_DG_ECHO
#define H_DG_ECHO

extern bool exit_flags[];

void init_flags();
void dg_cli(FILE*, int, const SA*, int);
void dg_echo(int, SA*, int);
void str_echo(int);
void recvfrom_int(int);
void sig_chld(int);

int Accept(int, SA*, socklen_t*);
int Bind(int, const SA*, socklen_t);
int Close(int);
int Connect(int, const SA*, int);
int Inet_pton(int, const char*, void*);
int Listen(int, int);
ssize_t Recvfrom(int, void*, size_t, int, struct sockaddr*, int*);
int Select(int, fd_set*, fd_set*, fd_set*, struct timeval*); 
ssize_t Sendto(int, const void*, size_t, int, const struct sockaddr*, int);
int Setsockopt(int, int, int, const void*, socklen_t);
int Socket(int, int, int);

char* Fgets(char*, int, FILE*);
int Fputs(const char*, FILE*);
ssize_t Read(int, void*, size_t);
ssize_t Write(int, const void*, size_t);

pid_t Fork();
void* Malloc(size_t);

ssize_t Readline(int, void*, size_t);
char* Sock_ntop(const struct sockaddr*, socklen_t);
ssize_t Writen(int, const void*, size_t);

#endif

