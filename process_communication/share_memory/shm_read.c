#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define PATHNAME "."
#define PROJ_ID 100

/*
 * 从共享内存中读，读完删除共享内存
 */

int main(int argc, const char *argv[])
{
    void *ret_shmaddr;
    key_t key;
    int shmid;
    char rbuffer[10];

    //创建key
    //key_t ftok(const char *pathname, int proj_id);
    key = ftok(PATHNAME, PROJ_ID);
    if(key == -1)
    {
        perror("ftok()");
        exit(EXIT_FAILURE);
    }

    //创建共享内存
    //int shmget(key_t key, size_t size, int shmflg);
    shmid = shmget(key, 10, IPC_EXCL);
    if(shmid == -1)
    {
        perror("shmget()");
        exit(EXIT_FAILURE);
    }
    printf("create share memory\n");

    //映射共享内存
    //void *shmat(int shmid, const void *shmaddr, int shmflg);
    ret_shmaddr = shmat(shmid, NULL, 0666);
    if(ret_shmaddr == (void *)-1)
    {
        perror("shmat()");
        exit(EXIT_FAILURE);
    }
    printf("ret_shmaddr : %p\n", ret_shmaddr);

    //从共享内存读
    memcpy((void *)rbuffer, ret_shmaddr, 10);
    printf("read from shm : %s\n", rbuffer);

    //解除映射
    //int shmdt(const void *shmaddr);
    if(shmdt(ret_shmaddr) == -1)
    {
        perror("shmdt()");
        exit(EXIT_FAILURE);
    }

    //删除共享内存
    //int shmctl(int shmid, int cmd, struct shmid_ds *buf);
    if(shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl_RMID");
        exit(EXIT_FAILURE);
    }

    return 0;
}
