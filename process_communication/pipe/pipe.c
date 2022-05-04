#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * 父进程向管道写 "hello hqyj"
 * 子进程从管道读 
 */

int main(int argc, const char *argv[])
{
    int pipefd[2];
    pid_t pid;
    int ret_pipe;
    
    //创建无名管道
    ret_pipe = pipe(pipefd);
    if(ret_pipe == -1)
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

    //创建子进程
    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)//子进程
    {
        ssize_t ret_read;
        char rbuffer[10];

        //关闭 写端
        close(pipefd[1]);

        sleep(2);
        ret_read = read(pipefd[0], rbuffer, 10);
        if(ret_read == -1)
        {
            perror("read()");
            exit(EXIT_FAILURE);
        }
        printf("child read from pipe : %s\n", rbuffer);

        //关闭 读端
        close(pipefd[0]);

        exit(EXIT_SUCCESS);
    }
    else//父进程
    {
        ssize_t ret_write;
        char wbuffer[] = "hello hqyj";
        //关闭 读端
        close(pipefd[0]);

        ret_write = write(pipefd[1], wbuffer, 10);
        if(ret_write == -1)
        {
            perror("write()");
            exit(EXIT_FAILURE);
        }
        printf("parent write to pipe : %s\n", wbuffer);

        //关闭 写端
        close(pipefd[1]);

        wait(pid, NULL, 0);

        exit(EXIT_SUCCESS);
    }

}
