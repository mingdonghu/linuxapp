#include "sem.h"
   
/*
   stdout : 对于所有的进程来说就是共享资源
   父进程 :
   "P start"
   ......
   "P end"
   子进程 :
   "C start"

   "C end"
   .父进程所有的信息输出后,表示访问共享资源结束
*/

int main(int argc,char *argv[])
{
    pid_t pid;  
    int semid;
    int shmid;
    
    semid = sem_init(1,1);

    pid = fork();

    if (pid == -1)
    {
        perror("fork()");   
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        for (;;)
        {
            sem_p(semid);   
            fprintf(stdout,"C start.\n");
            sleep(1);
            fprintf(stdout,"C end.\n");
            sem_v(semid);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        for (;;)
        {
            sem_p(semid);
            fprintf(stdout,"P start.\n");
            sleep(1);
            fprintf(stdout,"P end.\n");
            sem_v(semid);
        }
        wait(NULL);
    }


    return 0;
}
