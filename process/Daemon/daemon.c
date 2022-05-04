#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>

int main(int argc, const char *argv[])
{
    pid_t pid;
    int i;
    
    //��һ�� �� ���� ���¶����̡�
    pid = fork();           
    if(pid < 0)         
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if(pid > 0)
    {
        exit(EXIT_SUCCESS); //�������˳�
    }

    //�ڶ��� �� �����»Ự
    setsid();

    //������ �� �ı乤��Ŀ¼
    chdir("/");

    //���Ĳ� �� ��������
    umask(0);

    //���岽 �� �ر��ļ�������
    for(i = 0; i < getdtablesize(); i++)
    {
        close(i);
    }

    //������ɿ�ʼ����  
    
    //��ϵͳ��־����
    openlog("daemon_syslog", LOG_PID, LOG_DAEMON);

    int fd;
    fd = open("daemon.log", O_CREAT | O_WRONLY, 0600);
    if(fd < 0)
    {
        syslog(LOG_ERR, "open");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        write(fd, "hello", 5);
        sleep(1);
    }
    closelog();
    close(fd);
    return 0;
}
