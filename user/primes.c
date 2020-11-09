#include "kernel/types.h"
#include "user/user.h"

void close_pipe(int *p) {
  close(p[0]);
  close(p[1]);
}

void primes() {
  int n, p, len;
  int fd[2];

  // 从主进程读第一个数
  if ((len = read(0, &n, sizeof(int))) <= 0 ) {
    close(0);
    wait();
    exit();
  }
  // 向控制台输出
  printf("prime %d\n", n);
  
  pipe(fd);
  if (fork() == 0) {
    close(0);  //子进程 关闭文件描述符0
    dup(fd[0]);  //子进程将 管道读端口 拷贝到 描述符0
    close_pipe(fd);  //关闭 管道读写端口，子进程此时 只能对管道读数据
    primes(); 
  } else {
    close(1);  //主进程 关闭文件描述符1
    dup(fd[1]);  //主进程将 管道写端口 拷贝到 描述符1
    close_pipe(fd);  //关闭 管道读写端口，主进程此时 只能对管道写数据
    while ((len = read(0, &p, sizeof(int))) > 0 ) {
      if (p % n != 0) {
        write(1, &p, sizeof(int));
      }
    }
    if (len <= 0) {
      close(1);
      wait();
      exit();
    }
  } 
}

int main(void) {
  int i;
  int fd[2];
  pipe(fd);
  if (fork() == 0) {
    close(0);  //子进程 关闭文件描述符0
    dup(fd[0]);  //子进程将 管道读端口 拷贝到 描述符0
    close_pipe(fd);  //关闭 管道读写端口，子进程此时 只能对管道读数据
    primes();
  } else {
    close(1);  //主进程 关闭文件描述符1
    dup(fd[1]);  //主进程将 管道写端口 拷贝到 描述符1
    close_pipe(fd);  //关闭 管道读写端口，主进程此时 只能对管道写数据
    for (i = 2; i <= 35; i++) {
      write(1, &i, sizeof(int));  //向管道依次写入 2~35
    }
    close(1);  //关闭 文件描述符1，无指向管道的文件描述符，管道关闭
    wait();  //等待子进程结束
  }
  exit();
}