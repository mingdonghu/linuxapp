#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/* 写运行表
 * 1、子进程让自己停下
 * 2、父进程等子进程停下，让子进程继续
 * 3、子进程继续做自己的事，之后退出
 * 4、父进程等子进程退出
 * 5、父进程退出
 */

int main(int argc, const char *argv[])
{
    pid_t pid;

    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)//子进程
    {
        //子进程让自己停下来
        printf("child stop\n");
        raise(SIGSTOP);

        //子进程继续做自己的事情
        printf("hello hqyj\n");

        printf("child exit\n");
        exit(EXIT_SUCCESS);
    }
    else//父进程
    {
        //父进程等子进程停下
        waitpid(pid, NULL, 0 | WUNTRACED);

        //父进程让子进程继续
        printf("parent send to child continue\n");
        kill(pid, SIGCONT);

        //父进程等子进程退
        wait(NULL);

        printf("parent exit\n");
        exit(EXIT_SUCCESS);
    }
    return 0;
}
