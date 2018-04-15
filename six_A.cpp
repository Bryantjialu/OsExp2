/*A进程创建一个消息队列（邮箱）；
B进程向邮箱按顺序发送三条类型分别为111/222/333的消息，内容不作限定；
C进程按333/111/222的顺序读取消息；
D进程删除该消息队列（邮箱）。*/
#include<stdio.h>
#include<sys/msg.h>
#include<stdlib.h>
void main()
{
	int mq_id;
	if((mq_id = msgget(ftok(".",222),IPC_CREAT|0666))<0)
	{
		printf("error in msgget %s",__FILE__);
		exit(1);
	}
	printf("create msg queue : successful!");
	printf("get msg id is : %d\n",mq_id);
}
