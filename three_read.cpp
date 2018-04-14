#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<errno.h>
#include<unistd.h>
void main(){
	int shmid;
	if((shmid=shmget((key_t)1234,30,0666|IPC_CREAT))<0)	{
		printf("error in shmget(read)");
		exit(1);
	}
	char *shmptr;
	if((shmptr=shmat(shmid,0,SHM_RDONLY))==(void*)-1){
		printf("error in shmat(read),%X\n",(int)shmptr);		
		exit(1);
	}
	printf("read:start to sleep\n");
	sleep(4);
	printf("read: start to read\n");	
	printf("read data:%s\n",shmptr);
	if(shmdt(shmptr)== -1){
		printf("error in shmdt(read)");
	}	
	if(shmctl(shmid, IPC_RMID, 0) == -1) {  
      	fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
      	exit(1);  
    	}  
	sleep(5);
}
