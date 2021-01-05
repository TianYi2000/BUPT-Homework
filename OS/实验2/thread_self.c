/*thread.c*/
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
void *create(void *arg)
{
	printf("New thread......\n");
	printf("This thread's id is %u \n" ,(unsigned int)pthread_self());
	printf("This thread process pid is %d \n" ,getpid());
	return NULL ;
}
int main()
{
	pthread_t tid;
	int res;
	printf("Main thread is starting...\n");
	res=pthread_create(&tid, NULL ,create ,NULL);
	if(res)
	{
		printf("thread is not created...\n");
		return -1;
	}
	printf("The main process pid is %d \n" ,getpid());
	sleep(1);
	return 0 ;
}
