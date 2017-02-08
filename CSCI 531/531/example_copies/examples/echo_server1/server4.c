#include "/usr2/onyuksel/courses/531/examples/echo_server1/echo.h"

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
    ssize_t n;
    long arg1, arg2; 
    char line[MAXLINE];

    while (true) {
        if (Readline(sockfd, line, MAXLINE) == 0) return;

        if (sscanf(line, "%ld%ld", &arg1, &arg2) == 2) 
            snprintf(line, sizeof(line), "%ld\n", arg1 + arg2);
        else
            snprintf(line, sizeof(line), "input error\n");

        n = strlen(line);
        Writen(sockfd, line, n);
    }
}

void sig_chld(int signo)
{
    pid_t pid;
    Signal(SIGCHLD, sig_chld);

    pid = Wait(0);
    printf("child %d terminated\n", (int)pid);
}

