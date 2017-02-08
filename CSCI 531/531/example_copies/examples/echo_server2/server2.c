#include "/usr2/onyuksel/courses/531/examples/echo_server2/echo.h"

int main()
{
    ssize_t n;
    int i, nready; nfds_t maxi;
    int listenfd, connfd, sockfd;
    struct pollfd client[OPEN_MAX];
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

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; i++) client[i].fd = -1;
    maxi = 0;

    while (true) {
        nready = Poll(client, maxi+1, INFTIM);

        if (client[0].revents & POLLRDNORM) {
            connfd = Accept(listenfd, 0, 0);

            for (i = 1; i < OPEN_MAX && client[i].fd >= 0; i++);
            if (i < OPEN_MAX) client[i].fd = connfd;
            else { fprintf(stderr, "too many clients\n"); exit(1); }

            client[i].events = POLLRDNORM;
            if (i > maxi) maxi = i;
            nready--;
        }

        for (i = 1; i <= maxi && nready > 0; i++)
            if ((sockfd = client[i].fd) >= 0 &&
                (client[i].revents & (POLLRDNORM | POLLERR))) {
                if ((n = readline(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        Close(sockfd); client[i].fd = -1;
                    }
                    else { perror("readline error"); exit(1); }
                }
                else if (n == 0) {
                    Close(sockfd); client[i].fd = -1;
                }
                else Writen(sockfd, line, n);
                nready--;
            }
    }

    Close(listenfd); exit(0);
}

