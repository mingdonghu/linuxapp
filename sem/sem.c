#include "./sem.h"


//�ź�����ʼ��
int sem_init(int nsems, int value)
{
    key_t key;
    int semid;
    union semun s;
    int ret_semctlSETVAL;

    //��ȡkey
    key = ftok(PATHNAME, PROJ_ID);
    if(key == -1)
    {
        perror("ftok()");
        exit(EXIT_FAILURE);
    }

    //�����ź������ϣ��ź���һ��
    semid = semget(key, nsems, IPC_CREAT | 0644);
    if(semid == -1)
    {
        perror("semget()");
        exit(EXIT_FAILURE);
    }

    //������λ��
    s.val = value;
    ret_semctlSETVAL = semctl(semid, 0, SETVAL, s);
    if(ret_semctlSETVAL == -1)
    {
      perror("semctl SETVAL");
      exit(EXIT_FAILURE);
    }

    return semid;
}

//ռ��
void sem_p(int semid)
{
    struct sembuf semb;
    int ret_semop;

    semb.sem_num = 0;
    semb.sem_op = -1;
    semb.sem_flg = SEM_UNDO;

    ret_semop = semop(semid, &semb, 1);
    if(ret_semop == -1)
    {
        perror("semop P");
        exit(EXIT_FAILURE);
    }
}

//�ͷ�
void sem_v(int semid)
{
    struct sembuf semb;
    int ret_semop;

    semb.sem_num = 0;
    semb.sem_op =  1;
    semb.sem_flg = SEM_UNDO;

    ret_semop = semop(semid, &semb, 1);
    if(ret_semop == -1)
    {
        perror("semop V");
        exit(EXIT_FAILURE);
    }
}

void sem_del(int semid)
{
    int ret_semctlRMID;

    ret_semctlRMID = semctl(semid, 0, IPC_RMID, NULL);
    if(ret_semctlRMID == -1)
    {
        perror("semctl RMID");
        exit(EXIT_FAILURE);
    }

}

