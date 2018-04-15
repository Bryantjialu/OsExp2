//头文件shm_com_sem.h  
#include <fcntl.h>   
#include <sys/stat.h>  
#include <semaphore.h>    
  
#define LINE_SIZE 256  
#define NUM_LINE 16  
  
const char * queue_mutex="queue_mutex";  
const char * queue_empty="queue_empty";  
const char * queue_full="queue_full";  
//公用缓冲区    
struct shared_mem_st  
{  
    char buffer[NUM_LINE][LINE_SIZE];//缓冲数据组  
    int line_write;//读写指针  
    int line_read;  
};  
  
  
//producer.c  
  
#include <stdio.h>    
#include <sys/ipc.h>    
#include <sys/shm.h>    
#include <semaphore.h>    
#include <fcntl.h>    
#include <stdlib.h>    
#include <unistd.h>    
#include <string.h>  
  
#include "shm_com_sem.h"  
  
  
int main(void)  
{  
    void *shared_memory=(void *)0;//共享内存 缓冲区指针  
    struct shared_mem_st *shared_stuff;  
    char key_line[256];  
  
    int stmid;//共享内存id  
  
    sem_t *sem_queue,*sem_queue_empty,*sem_queue_full;//访问共享内存的信号量指针  
  
  
    stmid=shmget((key_t)1234,sizeof(struct shared_mem_st),0666|IPC_CREAT);//创建一个新的共享内存  
     if(stmid==-1)  
    {  
      perror("shmget failed");  
      exit(1);  
    }  
    if((shared_memory = shmat(stmid,0,0))<(void *)0){  //若共享内存区映射到本进程的进程空间失败  
        perror("shmat failed");    
        exit(1);    
    }   
  
    shared_stuff=(struct shared_mem_st *)shared_memory;//将缓冲区指针转换为shared_mem_st格式  
  
  
    sem_queue=sem_open("queue_mutex",O_CREAT,0644,1);  
    sem_queue_empty=sem_open("queue_empty",O_CREAT,0644,1);  
    sem_queue_full=sem_open("queue_full",O_CREAT,0644,1);//创建三个信号量  
  
  
    //读写指针初始化  
    shared_stuff->line_write=0;  
    shared_stuff->line_read=0;  
  
    char b[BUFSIZ];  
    int running=1;  
    while(running)  
    {  
        sleep(1);  
        sem_wait(sem_queue_empty);  
        sem_wait(sem_queue);//等待信号量输入  
        printf("Enter some text:");  
        fgets(b,BUFSIZ,stdin);  
        strncpy(shared_stuff->buffer[shared_stuff->line_write],b,LINE_SIZE);  
        shared_stuff->line_write=(shared_stuff->line_write+1)%NUM_LINE;//写指针增加  
        if(strncmp(b,"end",3)==0)//如果输入为end则退出  
        {  
             running=0;  
         }  
        sem_post(sem_queue);//发送信号量  
        sem_post(sem_queue_full);  
    }  
//为了防止结束后进程阻塞，所以再发送一次信号量和内容  
    sem_post(sem_queue);//发送信号量  
    sem_post(sem_queue_full);  
    strncpy(shared_stuff->buffer[shared_stuff->line_write],b,LINE_SIZE);  
    shared_stuff->line_write=(shared_stuff->line_write+1)%NUM_LINE;//写指针增加  
  
    if(shmdt(shared_memory)==-1)//解除映射  
    {  
        fprintf(stderr,"shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //删除共享内存区  
    if(shmctl(stmid,IPC_RMID,0)==-1)  
    {  
        fprintf(stderr,"shmctl failed\n");  
        exit(EXIT_FAILURE);  
    }  
}  