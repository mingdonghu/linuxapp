#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PATHNAME "."
#define PROJ_ID  100

/*
 * 创建消息队列,发送数据
 */

//发送消息结构体 msgp
struct msgbuf {
    long mtype;	 /* message type, must be > 0 */
    char mtext[1];	 /* message data */
};

int main(int argc, const char *argv[])
{
    key_t key;
    int msgid;
    struct msgbuf sbuffer;
    int ret_msgsnd;
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

    //发送消息
    sbuffer.mtype = 100;
    strcpy(sbuffer.mtext, "hello hqyj");
    ret_msgsnd = msgsnd(msgid, &sbuffer, 10, 0);
    if(ret_msgsnd == -1)
    {
        perror("msgsnd()");
        exit(EXIT_FAILURE);
    }
    printf("send to msg\n");

    //删除消息队列？
    

    return 0;
}
