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

/*B进程将A进程得到的消息队列的id值作为参数，
得到id值后向这个‘邮箱’发送三个消息
类型分别为111/222/333 */

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("只允许一个参数\n");
        exit(1);
    }
    printf("这里是进程B\n");

    int msgId = atoi(argv[1]);
    struct myMq msg1;
    struct myMq msg2;
    struct myMq msg3;

    msg1.mtype = 111;
    strcpy(msg1.mtext, "第一条消息的类型是111\n");
    int len1 = strlen(msg1.mtext);
    msgsnd(msgId, &msg1, len1, 0);
    printf("发送消息：%s(%d)\n", msg1.mtext, msg1.mtype);

    msg2.mtype = 222;
    strcpy(msg2.mtext, "第二条消息的类型是222\n");
    int len2 = strlen(msg2.mtext);
    msgsnd(msgId, &msg2, len2, 0);
    printf("发送消息：%s(%d)\n", msg2.mtext, msg2.mtype);

    msg1.mtype = 333;
    strcpy(msg3.mtext, "第三条消息的类型是333\n");
    int len3 = strlen(msg3.mtext);
    msgsnd(msgId, &msg3, len3, 0);
    printf("发送消息：%s(%d)\n", msg3.mtext, msg3.mtype);

    return 0;
}