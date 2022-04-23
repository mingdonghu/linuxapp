#ifndef __SEM_H
#define __SEM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PATHNAME "./"
#define PROJ_ID 200

union semun
{
    int val;
};

int sem_init(int nsems, int value);
void sem_p(int semid);
void sem_v(int semid);
void sem_del(int semid);


#endif

