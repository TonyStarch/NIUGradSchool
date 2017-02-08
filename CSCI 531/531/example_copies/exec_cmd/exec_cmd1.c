#include "/usr2/onyuksel/courses/531/common/531.h"

int main()
{
    pid_t pid;
    int status;
    char buf[MAXLINE];

    printf("%% "); /* print promt */
    while (fgets(buf, MAXLINE, stdin)) {
        buf[strlen(buf)-1] = 0; /* replace newline with null */

        if ((pid = fork()) < 0) {
            perror("fork error"); exit(1);
        }

        /* child */
        else if (pid == 0) {
            execlp(buf, buf, 0);
            fprintf(stderr, "couldn't execute: %s\n", buf);
            exit(2);
        }

        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            perror("waitpid error"); exit(3);
        }
        printf("%% ");
    }

    exit(0);
}

