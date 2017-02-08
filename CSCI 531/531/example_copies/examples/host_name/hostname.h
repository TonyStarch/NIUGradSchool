#include "/usr2/onyuksel/courses/531/common/531.h"
#include "/usr2/onyuksel/courses/531/common/wrapper.h"

#ifndef H_HOSTNAME
#define H_HOSTNAME

extern bool exit_flags[];

void init_flags();

int Connect(int, const SA*, socklen_t);
const char* Inet_ntop(int, const void*, char*, size_t);
int Inet_pton(int, const char*, void*);
int Socket(int, int, int);

ssize_t Read(int, void*, size_t);
int Fputs(const char*, FILE*);

char* Sock_ntop(const struct sockaddr*, socklen_t);

struct hostent* Gethostbyname(const char*);
struct servent* Getservbyname(const char*, const char*);

#endif

