#include "/usr2/onyuksel/courses/531/examples/dg_echo/dg_echo.h"

int main(int argc, char* argv[])
{
    int listenfd, connfd, udpfd;
    ssize_t n; socklen_t len;
    int nready, maxfdp1; pid_t childpid;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;
    char mesg[MAXLINE];
    fd_set rset;

    init_flags();
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    udpfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(udpfd, (SA*)&servaddr, sizeof(servaddr));

    signal(SIGCHLD, sig_chld);

    FD_ZERO(&rset);
    maxfdp1 = max(listenfd, udpfd) + 1;

    while (true) {
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);
        if ((nready = Select(maxfdp1, &rset, 0, 0, 0)) < 0) continue;

        if (FD_ISSET(listenfd, &rset)) {
            len = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA*)&cliaddr, &len);

            if ((childpid = Fork()) == 0) {
                Close(listenfd);
                str_echo(connfd);
                Close(connfd); exit(0);
            }
            Close(connfd);
        }

        if (FD_ISSET(udpfd, &rset)) {
            len = sizeof(cliaddr);
            n = Recvfrom(udpfd, mesg, MAXLINE, 0, (SA*)&cliaddr, &len); 
            Sendto(udpfd, mesg, n, 0, (SA*)&cliaddr, len);
        }
    }

    Close(listenfd); exit(0);
}

void str_echo(int sockfd)
{
    ssize_t n;
    char line[MAXLINE];

    while (true) {
        if ((n = Readline(sockfd, line, MAXLINE)) == 0) return;
        Writen(sockfd, line, n);
    }
}

void sig_chld(int signo)
{
    signal(SIGCHLD, sig_chld);
    waitpid(-1, 0, WNOHANG);
}

