#include "/usr2/onyuksel/courses/531/examples/echo_server1/echo.h"

int main(int argc, char* argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPaddress>", argv[0]);
        exit(1);
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);

    exit(0);
}

void str_cli(FILE* fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (Fgets(sendline, MAXLINE, fp)) {
        Writen(sockfd, sendline, strlen(sendline));

        if (!Readline(sockfd, recvline, MAXLINE)) {
            fprintf(stderr, "str_cli: server terminated prematurely\n");
            exit(1);
        }

        Fputs(recvline, stdout);
    }
}

