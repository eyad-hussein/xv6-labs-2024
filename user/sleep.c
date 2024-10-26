#include "kernel/types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: sleep <seconds>\n");
        exit(1);
    }

    int seconds = atoi(argv[1]);
    if (seconds < 0) {
        printf("seconds must be non-negative\n");
        exit(1);
    }

    sleep(seconds);
    exit(0);
}

