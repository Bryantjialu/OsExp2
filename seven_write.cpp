#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<fcntl.h>
#include<string.h>
#include<semaphore.h>

#define BUF_SIZE 1024

#define NBUFF 1

int main(void )
{
	int shm_id ;
	char msg[BUF_SIZE];
	if((shm_id=shmget(IPC_PRIVATE,BUF_SIZE,0666))<0)
	{
		printf("error in shmget");
		exit(1);
	}	
	printf("writer,shm id is :%d\n",shm_id);
	char *shm_ptr;
	if((shm_ptr=shmat(shm_id,0,0))==(void*)-1)
	{
		printf("error in shmat");
		exit(1);
	}
	sem_unlink("mutex");
	sem_unlink("full");
	sem_unlink("empty");
	sem_t *mutex = sem_open("mutex",O_CREAT,0666,1);
	sem_t *full  = sem_open("full",O_CREAT,0666,0);
	sem_t *empty = sem_open("empty",O_CREAT,0666,NBUFF);
	while(1)
	{
		printf("writer,pls enter the string:  \n");
		if((fgets(msg,BUF_SIZE-1,stdin))==NULL)
		{
			printf("error in fgets\n");
			exit(1);
		}
		
		sem_wait(empty);
		sem_wait(mutex);	
		strcpy(shm_ptr,msg);
		printf("writer,write :%s\n",shm_ptr);
		sem_post(mutex);
		sem_post(full);
		if(strncmp(msg,"//end",5)==0)
		{
			break;
		}
	}	
	return 0;
}
