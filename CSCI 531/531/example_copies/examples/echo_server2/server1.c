#include "/usr2/onyuksel/courses/531/examples/echo_server2/echo.h"

int main()
{
    ssize_t n;
    int i, maxi, maxfd;
    int listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    fd_set rset, allset;
    struct sockaddr_in servaddr;
    char line[MAXLINE];

    init_flags();
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    maxfd = listenfd; maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++) client[i] = -1;
    FD_ZERO(&allset); FD_SET(listenfd, &allset);

    while (true) {
        rset = allset;
        nready = Select(maxfd+1, &rset, 0, 0, 0);

        if (FD_ISSET(listenfd, &rset)) {
            connfd = Accept(listenfd, 0, 0);

            for (i = 0; i < FD_SETSIZE && client[i] >= 0; i++);
            if (i < FD_SETSIZE) client[i] = connfd;
            else { fprintf(stderr, "too many clients\n"); exit(1); }

            FD_SET(connfd, &allset);
            if (connfd > maxfd) maxfd = connfd;
            if (i > maxi) maxi = i;
            nready--;
        }

        for (i = 0; i <= maxi && nready > 0; i++) {
            sockfd = client[i];
            if (sockfd >= 0 && FD_ISSET(sockfd, &rset)) {
                if ((n = Readline(sockfd, line, MAXLINE)) == 0) {
                    FD_CLR(sockfd, &allset); client[i] = -1;
                    Close(sockfd);
                }
                else Writen(sockfd, line, n);
                nready--;
            }
        }
    }

    Close(listenfd); exit(0);
}

