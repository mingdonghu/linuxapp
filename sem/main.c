/*
 * ���ܣ�
 * ͨ���ź���ʵ�ָ��ӽ���ͬ��
 * */
#include "./sem.h"


int main(int argc, const char *argv[])
{
    pid_t pid;
    int semid;

    //0.�����ź���
    semid = sem_init(1, 1);

    pid = fork();  //�����ӽ���
    if(pid == -1)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) //�ӽ���
    {
        while(1)
        {
            //1.ռ����Դ
            sem_p(semid);
            //2.������
            printf("child P\n");
            sleep(1);
            printf("child V\n");
            //3.�ͷ���Դ
            sem_v(semid);
        }
        //4.�˳�
        exit(EXIT_SUCCESS);
    }
    else     //������ 
    {
        while(1)
        {
            //1.ռ����Դ
            sem_p(semid);
            //2.������
            printf("parent P\n");
            sleep(1);
            printf("parent V\n");
            //3.�ͷ���Դ
            sem_v(semid);
        }
    }
    //4.�ȴ��ӽ����˳�
    wait(NULL);

    //ɾ���ź�������
    sem_del(semid);

    //5.�˳�
    exit(EXIT_SUCCESS);

    return 0;
}
