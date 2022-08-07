#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char* args[MAXARG];

int
main(int argc, char* argv[])
{
    char* args[MAXARG];
    for(int i = 0; i < MAXARG; i++)
        args[i] = (char*)(malloc(32));

    int i;
    for(i = 1; i < argc; i++){
        args[i] = argv[i];
    }

    char buf;
    int idx = 0;
    while(read(0,&buf,1)){
        if(buf == ' ' || buf == '\n'){
            i++;
            idx = 0;
        }else{
            args[i][idx++] = buf;
        }
    }

    char* p[i-1];
    for(int j = 0; j < i-1; j++){
        p[j] = args[j+1];
    }

    if(fork() == 0){
        exec(args[1],p);
    }
    wait(0);

    exit(0);
}