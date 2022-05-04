#include "sem.h"

int sem_init(int nsems,int value)
{
    key_t key;
    int semid;//信号量集合的 ID

    key = ftok(PATHNAME,PROJ_ID);

    //创建一个信号量集合,包含 nsems 个信号量
    semid = semget(key, nsems, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget()"); 
        return -1;
    }
    //设置信号量的值(假定信号量集合包含一个1个信号量)   
    union semun s;
    s.val = value;
    if (semctl(semid,0,SETVAL,s) < 0)
    {
        perror("semctl()"); 
        return -1;
    }

    return semid;
}

int sem_p(int semid)
{
    struct sembuf sops;
    sops.sem_num = 0;//信号量的编号
    sops.sem_op = -1;//占用资源
    sops.sem_flg = SEM_UNDO;

    if (semop(semid,&sops,1) < 0)
    {
        perror("semop()");  
        return -1;
    }
    return 0;
}

int sem_v(int semid)
{
    struct sembuf sops; sops.sem_num = 0;//信号量的编号
    sops.sem_op = 1;//占用资源
    sops.sem_flg = SEM_UNDO;

    if (semop(semid,&sops,1) < 0)
    {
        perror("semop()");  
        return -1;
    }

    return 0;
}

int sem_del(int semid)
{
    if (semctl(semid,0,IPC_RMID,NULL) < 0)
    {
        return -1;
    }

    return 0;
}
