#include "/usr2/onyuksel/courses/531/examples/host_name/hostname.h"

int main(int argc, char** argv)
{
    int sockfd, n;
    char recvline[MAXLINE];
    struct sockaddr_in servaddr;
    struct in_addr** pptr;
    struct in_addr inetaddr, *inetaddrp[2];
    struct hostent* hp;
    struct servent* sp;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <service>\n", argv[0]);
        exit(1);
    }

    if (!(hp = Gethostbyname(argv[1]))) {
        if (Inet_pton(AF_INET, argv[1], &inetaddr) > 0) {
            inetaddrp[0] = &inetaddr; inetaddrp[1] = 0;
            pptr = inetaddrp;
        }
    }
    else pptr = (struct in_addr**) hp->h_addr_list;

    sp = Getservbyname(argv[2], "tcp");

    for ( ; *pptr; pptr++) {
        sockfd = Socket(AF_INET, SOCK_STREAM, 0);

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = sp->s_port;
        memcpy(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));
        printf("trying %s\n", Sock_ntop((SA*)&servaddr, sizeof(servaddr)));

        Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
        break;
    }
    if (!(*pptr)) fprintf(stderr, "unable to connect\n");

    while ((n = Read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        Fputs(recvline, stdout);
    }

    exit(0);
}

