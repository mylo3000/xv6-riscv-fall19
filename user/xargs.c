#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int i, j;
    char args[MAXARG][32];  //参数列表
    char buffer;  //从管道读入的字符
    char *p[MAXARG]; //存储参数表指针
    while (1)
    {
        i = 0;
        memset(args, 0, MAXARG*32);
        //传递指令中的参数
        for ( j = 1; j < argc; j++)
        {
            strcpy(args[i], argv[j]);
            i++;
        }

        j = 0;
        while (i < MAXARG - 1) //参数列表有空余
        {
            if (read(0, &buffer, 1) <= 0) //从管道读入一个字符
            {
                wait();
                exit();
            }
            if (buffer == '\n')
            {
                break;
            }
            if (buffer == ' ')
            {
                i++;
                j=0;
                continue;
            }
            args[i][j] = buffer;
            j++;
        }

        for (i = 0; i < MAXARG-1; i++) {
            p[i] = args[i];
        }
        p[MAXARG - 1] = 0; //最后一个参数argv[size-1]必须为0，否则将执行失败
        //执行指令
        if (fork() == 0)
        {
            exec(argv[1], p);
            exit();
        }
    }
    exit();
}