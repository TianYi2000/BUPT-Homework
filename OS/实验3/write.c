//write.c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
typedef struct{
	char name[8];
	int age;
}people;
int main(int argc,char** argv)
{
	int shm_id, i;
	key_t key;
	char temp[8];
	people* p_map;
	char pathname[30];
	strcpy(pathname,"/tmp");
	key=ftok(pathname,0x03);//利用ftok()函数创建key。
	if(key==-1)
	{
		perror("ftok error");
		return -1;
	}
	printf("key=%d\n",key);
	shm_id=shmget(key,4096,IPC_CREAT|IPC_EXCL|0600);
	//shmid是共享内存标识符。Shmget()创建共享内存
	if(shm_id==-1)
	{
		perror("shmgct error");
		return -1;
		printf("shm_id=%d\n",shm_id);
		p_map=(people*)shmat(shm_id,NULL,0); //映射共享内存 memset(temp,0x00,sizeof(temp));
		strcpy(temp,"test");
		temp[4]='0';
		for(i=0;i<3;i++)
		{
		temp[4]+=1;
		strncpy((p_map+i)->name,temp,5);
		(p_map+i)->age=0+i;
		}
	}
	shmdt(p_map);//用来断开与共享内存附加点的地址,禁本进程访问此片共享内存。
	return 0;
}