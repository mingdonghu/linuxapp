#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    int ret_exec;

#if 0
    if(argc < 2)
    {
        fprintf(stderr,"usage : %s < 指令名称 > \n",argv[0]);
        exit(EXIT_FAILURE);
    }
#endif

    char *const arg[] = {"ls", "-l", NULL};
    char *const envp[] = {"PATH=/tmp", "USER=harry", NULL};
    ret_exec = execve("/bin/ls", arg, envp);
    if(ret_exec < 0)
    {
        perror("exec ");
        exit(EXIT_FAILURE);
    }

    return 0;
}
