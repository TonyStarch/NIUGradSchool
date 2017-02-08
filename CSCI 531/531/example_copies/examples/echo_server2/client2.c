#include "/usr2/onyuksel/courses/531/examples/echo_server2/echo.h"

int main(int argc, char* argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPaddress>", argv[0]);
        exit(1);
    }

    init_flags();
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);

    Close(sockfd); exit(0);
}

void str_cli(FILE* fp, int sockfd)
{
    fd_set rset;
    int fd, maxfdp1; bool stdineof;
    char sendline[MAXLINE], recvline[MAXLINE];

    stdineof = false;
    FD_ZERO(&rset); fd = fileno(fp);
    maxfdp1 = max(fd, sockfd) + 1;

    while (true) {
        if (!stdineof) FD_SET(fd, &rset);
        FD_SET(sockfd, &rset);
        Select(maxfdp1, &rset, 0, 0, 0);

        if (FD_ISSET(sockfd, &rset)) {
            if (Readline(sockfd, recvline, MAXLINE) == 0) {
                if (stdineof) return;
                else {
                    fprintf(stderr,
                        "str_cli: server terminated prematurely\n");
                    exit(1);
                }
            }
            Fputs(recvline, stdout);
        }

        if (FD_ISSET(fd, &rset)) {
            if (!Fgets(sendline, MAXLINE, fp)) {
                stdineof = true;
                Shutdown(sockfd, SHUT_WR);
                FD_CLR(fd, &rset);
                continue;
            }
            Writen(sockfd, sendline, strlen(sendline));
        }
    }
}

