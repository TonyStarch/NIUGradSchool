#include "/usr2/onyuksel/courses/531/examples/dg_echo/dg_echo.h"

void init_flags()
{
    int i;

    for (i = 0; i < NFLAGS; i++)
        exit_flags[i] = true;
}

