#include "kernel/types.h"
#include "user.h"

void sieve(int read_fd) __attribute__((noreturn));

void generate_numbers(int start, int end, int write_fd) {
    for (int i = start; i <= end; i++) {
        write(write_fd, &i, sizeof(i));
    }
    close(write_fd);
}

void sieve(int read_fd) {
    int prime;
    if (read(read_fd, &prime, sizeof(prime)) == 0) {
        close(read_fd);
        exit(0);
    }
    printf("prime %d\n", prime);

    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[1]);
        sieve(p[0]);
    } else {
        close(p[0]);
        int num;
        while (read(read_fd, &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(p[1], &num, sizeof(num));
            }
        }
        close(read_fd);
        close(p[1]);
        wait(0);
        exit(0);
    }
}

int main() {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[1]);
        sieve(p[0]);
    } else {
        close(p[0]);
        generate_numbers(2, 280, p[1]);
        close(p[1]);
        wait(0);
        exit(0);
    }
}

