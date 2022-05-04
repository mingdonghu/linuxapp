#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * ��������ܵ�д "hello hqyj"
 * �ӽ��̴ӹܵ��� 
 */

int main(int argc, const char *argv[])
{
    int pipefd[2];
    pid_t pid;
    int ret_pipe;
    
    //���������ܵ�
    ret_pipe = pipe(pipefd);
    if(ret_pipe == -1)
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

    //�����ӽ���
    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)//�ӽ���
    {
        ssize_t ret_read;
        char rbuffer[10];

        //�ر� д��
        close(pipefd[1]);

        sleep(2);
        ret_read = read(pipefd[0], rbuffer, 10);
        if(ret_read == -1)
        {
            perror("read()");
            exit(EXIT_FAILURE);
        }
        printf("child read from pipe : %s\n", rbuffer);

        //�ر� ����
        close(pipefd[0]);

        exit(EXIT_SUCCESS);
    }
    else//������
    {
        ssize_t ret_write;
        char wbuffer[] = "hello hqyj";
        //�ر� ����
        close(pipefd[0]);

        ret_write = write(pipefd[1], wbuffer, 10);
        if(ret_write == -1)
        {
            perror("write()");
            exit(EXIT_FAILURE);
        }
        printf("parent write to pipe : %s\n", wbuffer);

        //�ر� д��
        close(pipefd[1]);

        wait(pid, NULL, 0);

        exit(EXIT_SUCCESS);
    }

}
