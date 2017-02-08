#include "/usr2/onyuksel/courses/531/examples/echo_server1/echo.h"
#include "/usr2/onyuksel/courses/531/examples/echo_server1/sum.h"

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
    char line[MAXLINE];
    struct args args;
    struct result result;

    while (Fgets(line, MAXLINE, fp)) {
        if (sscanf(line, "%ld%ld", &args.arg1, &args.arg2) != 2) {
            snprintf(line, sizeof(line), "input error\n"); continue;
        }
        Writen(sockfd, &args, sizeof(args));

        if (Readn(sockfd, &result, sizeof(result)) == 0) {
            fprintf(stderr, "str_cli: server terminated prematurely\n");
            exit(1);
        }

        printf("%ld\n", result.sum);
    }
}

