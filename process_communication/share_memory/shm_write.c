#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define PATHNAME "."
#define PROJ_ID 100

/*
 * 创建共享内存并向里面写“hello hqyj”
 */

int main(int argc, const char *argv[])
{
    void *shmaddr;
    key_t key;
    int shmid;

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
    shmid = shmget(key, 10, IPC_CREAT | 0666);
    if(shmid == -1)
    {
        perror("shmget()");
        exit(EXIT_FAILURE);
    }
    printf("create share memory\n");

    //映射共享内存
    //void *shmat(int shmid, const void *shmaddr, int shmflg);
    shmaddr = shmat(shmid, NULL, 0666);
    if(shmaddr == (void *)-1)
    {
        perror("shmat()");
        exit(EXIT_FAILURE);
    }
    printf("shmaddr : %p\n", shmaddr);

    //向共享内存写“hello hqyj”
    memcpy(shmaddr, "hello hqyj", 10);

    //解除映射
    //int shmdt(const void *shmaddr);
    if(shmdt(shmaddr) == -1)
    {
        perror("shmdt()");
        exit(EXIT_FAILURE);
    }

    //删除共享内存？

    return 0;
}
