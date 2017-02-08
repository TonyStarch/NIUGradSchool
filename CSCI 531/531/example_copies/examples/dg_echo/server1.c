#include "/usr2/onyuksel/courses/531/examples/dg_echo/dg_echo.h"

int main(int argc, char* argv[])
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

	init_flags();
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(sockfd, (SA*)&servaddr, sizeof(servaddr));

    dg_echo(sockfd, (SA*)&cliaddr, sizeof(cliaddr));

    Close(sockfd); exit(0);
}

void dg_echo(int sockfd, SA* pcliaddr, int clilen)
{
    int n, len;
    char mesg[MAXLINE];

    while (true) {
        len = clilen;
        n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        Sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}

