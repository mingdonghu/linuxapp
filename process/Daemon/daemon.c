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
    
    //第一步 ： 制造 “孤儿进程”
    pid = fork();           
    if(pid < 0)         
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if(pid > 0)
    {
        exit(EXIT_SUCCESS); //父进程退出
    }

    //第二步 ： 创建新会话
    setsid();

    //第三步 ： 改变工作目录
    chdir("/");

    //第四步 ： 重设掩码
    umask(0);

    //第五步 ： 关闭文件描述符
    for(i = 0; i < getdtablesize(); i++)
    {
        close(i);
    }

    //创建完成开始工作  
    
    //打开系统日志服务
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
