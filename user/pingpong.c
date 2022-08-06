#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
    int p[2];

    pipe(p);

    char ping, pong;

    if (fork() == 0) {
        read(p[0], (void *)&ping, 1);
        write(p[1], (const void *)('p'), 1);
        printf("%d: received ping\n", getpid());
        exit(0);
    } else {
        write(p[1], (const void *)('p'), 1);
        wait(0);
        read(p[0], (void *)&pong, 1);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}