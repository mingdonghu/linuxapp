#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, const char *argv[])
{
    pid_t pid;

    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        sleep(5);

        printf("1 pid = %d\n", getpid());

        while(1);

        exit(EXIT_SUCCESS);
    }
    else
    {
        pid_t pid_2;
        pid_2 = fork();
        if(pid_2 == 0)
        {
            sleep(3);

            printf("2 pid = %d\n", getpid());

            exit(EXIT_SUCCESS);
        }
        else
        {
            pid_t ret_pid = 0;

            while(ret_pid == 0)
            {
                //pid_t waitpid(pid_t pid, int *status, int options);
                ret_pid = waitpid(pid_2, NULL, WNOHANG);
                printf("ret_pid = %d\n", ret_pid);
                sleep(1);
                if(ret_pid == pid)
                {
                    printf("child 1 process exited\n");
                    break;
                }
                else if(ret_pid == pid_2)
                {
                    printf("child 2 process exited\n");
                    break;
                }
                else
                {
                    printf("child is not exit\n");
                }
            }

        }
        
        wait(NULL);
    }

    exit(EXIT_SUCCESS);
}
