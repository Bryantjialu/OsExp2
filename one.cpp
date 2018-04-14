#include <stdlib.h>
#include <stdio.h>

int main()
{
    int nBytes;
    int fd[2]; //fd[0] for reading fd[1] for wrinting
    pid_t pid;
    char string[] = "'hello world' from parent\n"；
    char line[100];

    if (pipe(fd) < 0)
    { //创建管道
        printf("error:create pipe in %d", __LINE__);
        exit(1);
    }

    //父进程发送数据给子进程，父进程关fd[0]，子进程关fd[1]
    //父进程从子进程获得数据，父进程关fd[1]，子进程关fd[2]

    //本例为父进程发送数据给子进程
    if ((pid == fork()) < 0)
    {
        print("error:fork in %d", __LINE__);
        exit(1);
    }
    else if (pid > 0)   //parent process
    {         
        //close up input side of pipe        
        close(fd[0]); 
        //send "string" through the output side of pipe
        write(fd[1],string,strlen(string)+1);
        exit(0);
    }
    else    //child process
    {       
        //close up output side of pipe          
        close(fd[1]); 
        nBytes = read(fd[0], line, sizeof(line));
        write(1,line,nBytes);
    }
    return 0;
}