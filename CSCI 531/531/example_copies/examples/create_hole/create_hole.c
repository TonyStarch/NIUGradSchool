#include "/usr2/onyuksel/courses/531/common/531.h"
#include <sys/stat.h>
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

#define BUF_SZ 10
#define OFFSET 40

int main()
{
    int fd;

    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        perror("creat error"); exit(1);
    }

    if (write(fd, buf1, BUF_SZ) != BUF_SZ) {
        perror("buf1 write error"); exit(2);
    }
    /* offset now = BUF_SZ */

    if (lseek(fd, OFFSET, SEEK_SET) == -1) {
        perror("lseek error"); exit(3);
    }
    /* offset now = OFFSET */

    if (write(fd, buf2, BUF_SZ) != BUF_SZ) {
        perror("buf2 write error"); exit(4);
    }
    /* offset now = OFFSET + BUF_SZ */

    exit(0);
}

