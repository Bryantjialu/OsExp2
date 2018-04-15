//customer.c  
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
    void * shared_memory=(void *)0;  
    struct shared_mem_st *shared_stuff;  
  
    int stmid;  
    int num_read;  
    pid_t fork_result;  
    sem_t *sem_queue,*sem_queue_empty,*sem_queue_full;  
      
    stmid=shmget((key_t)1234,sizeof(struct shared_mem_st),0666|IPC_CREAT);//获得已创建共享内存  
     if(stmid==-1)  
    {  
      perror("shmget failed");  
      exit(1);  
    }  
    if((shared_memory = shmat(stmid,0,0))<(void *)0){  //若共享内存区映射到本进程的进程空间失败  
        perror("shmat failed");    
        exit(1);    
    }   
  
    shared_stuff=(struct shared_mem_st *)shared_memory;  
  
    //获取三个信号量  
    sem_queue=sem_open("queue_mutex",0);  
    sem_queue_empty=sem_open("queue_empty",0);  
    sem_queue_full=sem_open("queue_full",0);  
  
  
    //创建两个进程  
    fork_result=fork();  
    if(fork_result==-1)  
        fprintf(stderr,"fork failed\n");  
    int running=1;  
    if(fork_result==0)//子进程  
    {  
        while(running)  
        {  
            sem_wait(sem_queue_full);  
            sem_wait(sem_queue);//等待信号量  
            sleep(2);  
            printf("child pid is %d,you wrote:%s\n",getpid(),shared_stuff->buffer[shared_stuff->line_read]);//输出进程号和消息内容  
              
              
            if(strncmp(shared_stuff->buffer[shared_stuff->line_read],"end",3)==0)//如果为end则退出  
            {   running=0;  
            }     
        //  printf("%d",running);  
            shared_stuff->line_read=(shared_stuff->line_read+1)%NUM_LINE;//读指针改变  
            sem_post(sem_queue);//发送信号量  
            sem_post(sem_queue_empty);  
        }  
        sem_unlink(queue_mutex);  
        sem_unlink(queue_empty);  
        sem_unlink(queue_full);  
    }  
    else//父进程  
    {   while(running)  
        {     
            sem_wait(sem_queue_full);  
            sem_wait(sem_queue);//等待信号量  
            sleep(2);  
            printf("parent pid is %d,you wrote:%s\n",getpid(),shared_stuff->buffer[shared_stuff->line_read]);//输出进程号和消息内容  
              
            if(strncmp(shared_stuff->buffer[shared_stuff->line_read],"end",3)==0)//如果为end则退出  
            {  
             running=0;  
            }  
        //  printf("%d",running);  
            shared_stuff->line_read=(shared_stuff->line_read+1)%NUM_LINE;//读指针改变  
            sem_post(sem_queue);//发送信号量  
            sem_post(sem_queue_empty);  
                  
        }  
     
        sem_unlink(queue_mutex);  
        sem_unlink(queue_empty);  
        sem_unlink(queue_full);  
    }  
    waitpid(fork_result,NULL,0);  
    exit(EXIT_SUCCESS);  
}  
