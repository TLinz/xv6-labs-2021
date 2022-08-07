#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    *(buf + strlen(p)) = '\0';
    return buf;
}

void
find(char *path, char *target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            if(strcmp(fmtname(path), target) == 0){
                printf("%s\n", path);
            }
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }

                if(st.type == T_FILE){
                    if(strcmp(fmtname(buf), target) == 0){
                        printf("%s\n", buf);
                    }
                } else {
                    if(strcmp(fmtname(buf), target) == 0){
                        printf("%s\n", buf);
                    }
                    if(strcmp(fmtname(buf), ".") == 0 || strcmp(fmtname(buf),"..") == 0){
                        continue;
                    }
                    find(buf, target);
                }
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    char *path = argv[1];
    char *target = argv[2];

    find(path, target);
    exit(0);
}