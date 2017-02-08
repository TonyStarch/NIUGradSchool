#include "/usr2/onyuksel/courses/531/common/531.h"
#include <dirent.h>

int main(int argc, char* argv[])
{
    DIR* dp;
    struct dirent* dirp;

    if (argc != 2) {
        fprintf(stderr,
            "a single argument (directory name) is required\n");
        exit(1);
    }

    if (!(dp = opendir(argv[1]))) {
        fprintf(stderr, "can't open %s: %s\n", argv[1],
            strerror(errno));
        exit(2);
    }

    while ((dirp = readdir(dp)))
        printf("%s\n", dirp->d_name);

    closedir(dp);
    exit(0);
}

