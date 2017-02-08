#include "/usr2/onyuksel/courses/531/examples/host_name/hostname.h"

void init_flags()
{
    int i;

    for (i = 0; i < NFLAGS; i++)
        exit_flags[i] = true;

	exit_flags[GETHOSTBYNAME] = false;
}

