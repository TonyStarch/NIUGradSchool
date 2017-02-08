#include "/usr2/onyuksel/courses/531/examples/dg_echo/dg_echo.h"

int main(int argc, char* argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPaddress>", argv[0]);
        exit(1);
    }

    init_flags();
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    dg_cli(stdin, sockfd, (SA*)&servaddr, sizeof(servaddr));

    Close(sockfd); exit(0);
}

void dg_cli(FILE* fp, int sockfd, const SA* pservaddr, int servlen)
{
    int n;
    socklen_t len;
    char sendline[MAXLINE], recvline[MAXLINE];
    struct sockaddr* preply_addr;

    preply_addr = Malloc(servlen);

    while (Fgets(sendline, MAXLINE, fp)) {
        Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        len = servlen;
        n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
        if (len != servlen || memcmp(pservaddr, preply_addr, len)) {
            printf("reply from %s (ignored)\n", Sock_ntop(preply_addr,
                len)); continue;
        }

        recvline[n] = 0;
        Fputs(recvline, stdout);
    }
}

