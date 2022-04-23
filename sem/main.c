/*
 * 功能：
 * 通过信号量实现父子进程同步
 * */
#include "./sem.h"


int main(int argc, const char *argv[])
{
    pid_t pid;
    int semid;

    //0.创建信号量
    semid = sem_init(1, 1);

    pid = fork();  //创建子进程
    if(pid == -1)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) //子进程
    {
        while(1)
        {
            //1.占用资源
            sem_p(semid);
            //2.做事情
            printf("child P\n");
            sleep(1);
            printf("child V\n");
            //3.释放资源
            sem_v(semid);
        }
        //4.退出
        exit(EXIT_SUCCESS);
    }
    else     //父进程 
    {
        while(1)
        {
            //1.占用资源
            sem_p(semid);
            //2.做事情
            printf("parent P\n");
            sleep(1);
            printf("parent V\n");
            //3.释放资源
            sem_v(semid);
        }
    }
    //4.等待子进程退出
    wait(NULL);

    //删除信号量集合
    sem_del(semid);

    //5.退出
    exit(EXIT_SUCCESS);

    return 0;
}
