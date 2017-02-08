#include "/usr2/onyuksel/courses/531/examples/echo_server1/echo.h"

int main()
{
    int listenfd, connfd;
    pid_t childpid;
    struct sockaddr_in servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

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
    char line[MAXLINE];

    while (true) {
        if (!(n = Readline(sockfd, line, MAXLINE))) return;
        Writen(sockfd, line, n);
    }
}

