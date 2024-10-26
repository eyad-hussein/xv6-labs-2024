#include "kernel/types.h"
#include "user.h"

int main() {
    int p1[2], p2[2];
    char buf[1];
    int pid;

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        printf("pipe creation failed\n");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        printf("fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        close(p1[1]);
        close(p2[0]);

        if (read(p1[0], buf, 1) != 1) {
            printf("child: read error\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());

        if (write(p2[1], buf, 1) != 1) {
            printf("child: write error\n");
            exit(1);
        }

        close(p1[0]);
        close(p2[1]);
        exit(0);
    } else {
        close(p1[0]);
        close(p2[1]);

        buf[0] = 'A';
        if (write(p1[1], buf, 1) != 1) {
            printf("parent: write error\n");
            exit(1);
        }

        if (read(p2[0], buf, 1) != 1) {
            printf("parent: read error\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());

        close(p1[1]);
        close(p2[0]);
        wait(0);
        exit(0);
    }
}

