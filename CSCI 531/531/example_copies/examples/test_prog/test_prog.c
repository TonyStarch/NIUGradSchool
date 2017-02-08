#include "/usr2/onyuksel/courses/531/common/531.h"

#define BUFSIZE 1

int main()
{
    int  n;
    char buf[BUFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n) {
            perror("write error"); exit(1);
        }

    if (n < 0) { perror("read error"); exit(2); }

    exit(0);
}

