#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<errno.h>
#include<unistd.h>
void main(){
	int shmid ;
	if((shmid=shmget((key_t)1234,30,0666|IPC_CREAT))<0){
		printf("error in shmget(write)\n");
		exit(1);
	}
	char *shmptr;
	if((shmptr=shmat(shmid,NULL,0))== (void*)-1)	{
		printf("error in shmat(write)\n");
		exit(1);	
	}
	printf("write :start to write:\n");
	strcpy(shmptr,"hello from write\n");	//写入数据
	printf("write success!\n");	
	if(shmdt(shmptr)==-1)	{	
		printf("error in shmdt (write)");
	}
	sleep(5);
}
