#include "/usr2/onyuksel/courses/531/common/531.h"

#define s_shrt sizeof(short)

int main(int argc, char* argv[])
{
    union {
        short s;
        char  c[s_shrt];
    } un;

    printf("sizeof(short) = %d\n", s_shrt);

    un.s = 0x0102;
    if (s_shrt == 2) {
        if (un.c[0] == 1 && un.c[1] == 2)
            printf("big-endian\n");
        else if (un.c[0] == 2 && un.c[1] == 1)
            printf("little-endian\n");
        else
            printf("unknown\n");
    }

    exit(0);
}

