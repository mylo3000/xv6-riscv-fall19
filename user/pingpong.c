#include "kernel/types.h"
#include "user/user.h"

int main() {
    int fd1[2],fd2[2];
    char buffer[] = {'0'};
    long length = sizeof(buffer);
    //0-read, 1-write
    pipe(fd1);  //子进程读，父进程写
    pipe(fd2);  //父进程读，子进程写
    if (fork() == 0)
    {
        close(fd1[1]);
        close(fd2[0]);
        read(fd1[0], buffer, length);
        printf("%d: received pong\n", getpid());
        write(fd2[1], buffer, length);
    }
    else
    {
        close(fd1[0]);
        close(fd2[1]);
        write(fd1[1], buffer, length);
        printf("%d: received ping\n", getpid());
        read(fd2[0], buffer, length);
    }
    exit();
}