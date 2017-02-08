#include "/usr2/onyuksel/courses/531/examples/dg_echo/dg_echo.h"

#define NO_DG  50000
#define DG_LEN  1400

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
    int i;
    char sendline[DG_LEN];

    for (i = 0; i < NO_DG; i++)
        Sendto(sockfd, sendline, DG_LEN, 0, pservaddr, servlen);
}

