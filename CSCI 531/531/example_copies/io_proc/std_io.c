#include "/usr2/onyuksel/courses/531/common/531.h"

int main()
{
    int c;

    while ((c = getc(stdin)) != EOF)
        if (putc(c, stdout) == EOF) {
            perror("putc error"); exit(1);
        }

    if (ferror(stdin)) { perror("getc error"); exit(2); }

    exit(0);
}

