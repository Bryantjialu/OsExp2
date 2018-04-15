#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
struct myMq
{
    /*
    mtype的值：
    = 0：接受该队列中的第一条信息
    < 0: 接收小于该值的绝对值得消息类型
    > 0: 接受指定类型的消息，即该值消息
    */
    long mtype;      //消息类型
    char mtext[512]; //消息文本
};
/*进程C像进程B一样，从参数处得到一个消息队列的id，
然后以他调用一个msgrcv函数，按序读取消息类型为333/111/222的消息*/
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("只允许一个参数\n");
        exit(1);
    }

    printf("这里是进程C\n");
    int msgId = atoi(argv[1]);
    struct myMq msg;
    int len = 512;
    msgrcv(msgId, &msg, len, 333, 0);
    printf("收到消息：%s\n",msg.mtext);

    msgrcv(msgId, &msg, len, 111, 0);
    printf("收到消息：%s\n",msg.mtext);

    msgrcv(msgId, &msg, len, 222, 0);
    printf("收到消息：%s\n",msg.mtext);
    
    return 0;
}