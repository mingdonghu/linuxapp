#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, const char *argv[])
{
    pid_t pid;
    printf("hello before\n");

    pid = fork();
    if(pid == 0)
    {
        printf("hello child\n");
    }
    else
    {
        printf("hello parent\n");
        wait(NULL);
    }

    printf("hello later\n");

    return 0;
}
