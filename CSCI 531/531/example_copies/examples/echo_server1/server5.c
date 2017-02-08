#include "/usr2/onyuksel/courses/531/examples/echo_server1/echo.h"
#include "/usr2/onyuksel/courses/531/examples/echo_server1/sum.h"

long convert(long);

int main()
{
    int listenfd, connfd;
    pid_t childpid;
    struct sockaddr_in servaddr;

    init_flags();
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    Signal(SIGCHLD, sig_chld);

    while (true) {
        connfd = Accept(listenfd, 0, 0);

        if ((childpid = Fork()) == 0) {
            Close(listenfd);
            str_echo(connfd);
            Close(connfd); exit(0);
        }
        Close(connfd);
    }

    Close(listenfd); exit(0);
}

void str_echo(int sockfd)
{
    struct args args;
    struct result result;

    while (true) {
        if (Readn(sockfd, &args, sizeof(args)) == 0) return;
        result.sum = convert(args.arg1) + convert(args.arg2);
        result.sum = convert(result.sum);
        Writen(sockfd, &result, sizeof(result));
    }
}

void sig_chld(int signo)
{
    pid_t pid;
    Signal(SIGCHLD, sig_chld);

    pid = Wait(0);
    printf("child %d terminated\n", (int)pid);
}

long convert(long x)
{
    long first  = (x & 0xff000000) >> 24,
         second = (x & 0x00ff0000) >> 8,
         third  = (x & 0x0000ff00) << 8,
         fourth = (x & 0x000000ff) << 24;

    return (first | second | third | fourth);
}

