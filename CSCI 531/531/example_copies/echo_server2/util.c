#include "/usr2/onyuksel/courses/531/examples/echo_server2/echo.h"

void init_flags()
{
    int i;

    for (i = 0; i < NFLAGS; i++)
        exit_flags[i] = true;
}

