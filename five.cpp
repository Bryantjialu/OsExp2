#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
/*进程A创建子进程B, 子进程B创建孙进程C，
AB间用管道1通信，BC间用管道2通信，实现进程A将一个消息字符串发送到C进程，
并有C进程打印出来以验证通信成功。
要求：管道2只对BC可见，即对A是不可见的。*/
int main()
{
    pid_t pid;
    char buf[128]; //消息的字符数组
    int fdsAB[2];  //管道1的进程A和进程B

    pipe(fdsAB);
    //进程A生成子进程B，所以AB都有管道fdsAB
    pid = fork();

    if (pid)
    {
        printf("这是进程A\n");
        //给字符数组赋值
        strcpy(buf, "Hello from A\n");
        printf("进程A发送消息给进程B\n");
        //进程A通过管道1与进程B通信
        write(fdsAB[1], buf, sizeof(buf));
        sleep(1);
    }
    else if (pid == 0)
    {
        //子进程B
        printf("这是进程B\n");
        //创建管道2，因为管道2只对BC可见所以在这个作用域里面声明
        int fdsBC[2];

        pipe(fdsBC);
        //子进程B创建孙进程C
        pid = fork();
        if (pid)
        {
            printf("进程B准备接受来自A的信息\n");
            //进程B通过管道1从父进程A接收信息
            read(fdsAB[0], buf, sizeof(buf));
            printf("进程B准备将进程A的信息发送给进程C\n");
            //进程B通过管道2向子进程C传递父进程A的消息
            write(fdsBC[1], buf, sizeof(buf));
            sleep(1);
        }
        else if (pid == 0)
        {
            sleep(1);
            printf("这里是进程C\n");
            printf("进程C准备接受进程B发送的信息\n");
            //进程C通过管道2从父进程B接收信息
            read(fdsBV[0],buf,sizeof(buf));
            printf("grandson C接收的信息：%s\n",buf);
        }
    }

    return 0;
}