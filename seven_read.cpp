#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<errno.h>
#include<semaphore.h>
#include<string.h>
#include<fcntl.h>
#define BUF_SIZE 1024

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("invaild arguement\n");
		exit(1);
	}
	int shm_id = atoi(argv[1]);
	char *shm_ptr;	
	if((shm_ptr=shmat(shm_id,0,0))==(void*)-1)
	{
		printf("error in shmat\n");
		exit(1);
	}
	sem_t *mutex = sem_open("mutex",O_CREAT);
	sem_t *full  = sem_open("full",O_CREAT);
	sem_t *empty = sem_open("empty",O_CREAT);

	pid_t pid;
	if((pid=fork())<0)
	{
		printf("error in fork()\n");
		exit(1);
	}
	else if(pid>0)
	{
		while(1)
		{
			sem_wait(full);
			sem_wait(mutex);
			printf("process %d receive message :%s\n",getpid(),shm_ptr);
			strcpy(shm_ptr,"");
			sem_post(mutex);
			sem_post(empty);
		}	
	}
	else
	{
		while(1)
		{
			sem_wait(full);
			sem_wait(mutex);
			printf("process %d receive message :%s\n",getpid(),shm_ptr);
			strcpy(shm_ptr,"");	
			sem_post(mutex);
			sem_post(empty);
		}
	}	
}
