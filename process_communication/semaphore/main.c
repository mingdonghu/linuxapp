#include "sem.h"
   
/*
   stdout : �������еĽ�����˵���ǹ�����Դ
   ������ :
   "P start"
   ......
   "P end"
   �ӽ��� :
   "C start"

   "C end"
   .���������е���Ϣ�����,��ʾ���ʹ�����Դ����
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
