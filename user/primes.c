#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// p_read is the left side of pipe which is created by left process.
void primes(int p_read) {
    int p[2];
    pipe(p); // Create a pipe to transfer current primes to next process.

    int prime, n;
    if (read(p_read, (void *)&prime, 1)) {
        printf("prime %d\n", prime);
    } else {
        close(p_read);
        close(p[1]);
        exit(0);
    }

    if (fork() == 0) {
        close(p[1]); //*
        primes(p[0]);
    }

    while(read(p_read, (void *)&n, 1)) {
        if (n % prime == 0) {
            continue;
        }
        write(p[1], (const void *)&n, 1);
    }

    close(p_read);
    close(p[1]);
    wait(0); //*
    exit(0);
}

int
main(int argc, char **argv)
{
    int p[2];
    pipe(p);
    int up_bound = 35;

    // Feed 2-35 to first process.
    if (fork() == 0) {
        close(p[1]); //*
        primes(p[0]);
    }
    for (int i = 2; i <= up_bound; i++) {
        write(p[1], (const void *)&i, 1);
    }
    close(p[1]);
    close(p[0]);

    wait(0); //*
    exit(0);
}