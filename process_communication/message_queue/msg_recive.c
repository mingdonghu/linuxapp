#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PATHNAME "."
#define PROJ_ID  100

/*
 * 从消息队列上接收消息
 */

//接收结构体 msgp
struct msgbuf {
    long mtype;	 /* message type, must be > 0 */
    char mtext[10];	 /* message data */
};

int main(int argc, const char *argv[])
{
    key_t key;
    int msgid;
    struct msgbuf rbuffer;
    int ret_msgrcv;
    int ret_msgctl_RMID;
    //创建key
    //key_t ftok(const char *pathname, int proj_id);
    key = ftok(PATHNAME, PROJ_ID);
    if(key == -1)
    {
        perror("ftok()");
        exit(EXIT_FAILURE);
    }

    //创建消息队列
    //int msgget(key_t key, int msgflg);
    msgid = msgget(key, IPC_CREAT | 0666);
    if(msgid == -1)
    {
        perror("msgget()");
        exit(EXIT_FAILURE);
    }
    printf("create msg\n");

    //接收消息
    ret_msgrcv = msgrcv(msgid, &rbuffer, 10, 100, 0);
    if(ret_msgrcv == -1)
    {
        perror("msgrcv()");
        exit(EXIT_FAILURE);
    }
    printf("recive msg type : %ld text: %s\n", rbuffer.mtype, rbuffer.mtext);

    //删除消息队
    //int msgctl(int msqid, int cmd, struct msqid_ds *buf);
    ret_msgctl_RMID = msgctl(msgid, IPC_RMID, NULL);
    if(ret_msgctl_RMID == -1)
    {
        perror("msgctl_RMID");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
