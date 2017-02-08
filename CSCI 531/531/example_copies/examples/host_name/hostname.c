#include "/usr2/onyuksel/courses/531/examples/host_name/hostname.h"

int main(int argc, char** argv)
{
    char *ptr, **pptr;
    char str[INET_ADDRSTRLEN];
    struct hostent* hptr;

    while (--argc > 0) {
        ptr = *++argv;
        if (!(hptr = Gethostbyname(ptr))) continue;

        printf("official hostname: %s\n", hptr->h_name);

        for (pptr = hptr->h_aliases; *pptr; pptr++)
            printf("\talias: %s\n", *pptr);

        switch (hptr->h_addrtype) {
            case AF_INET:
                for (pptr = hptr->h_addr_list; *pptr; pptr++)
                    printf("\taddress: %s\n",
                        Inet_ntop(hptr->h_addrtype, *pptr, str,
                            sizeof(str))); break;
            default:
                Perror("unknown address type");
                break;
        }
    }

    exit(0);
}

