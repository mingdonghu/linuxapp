#ifndef _SEM_H_
#define _SEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
    
#define PATHNAME "."
#define PROJ_ID 100

union semun
{
    int val;
};

int sem_init(int nsems,int value);//创建并初始化信号量集合
int sem_p(int semid);//占用资源
int sem_v(int semid);//释放资源
int sem_del(int semid);//删除信号量集合

#endif
