/* thread_exit.c文件*/
#include<stdio.h>
#include<pthread.h>
/*进程函数*/
void *create(void *arg)
{
	printf("New thread is created...\n");
	pthread_exit((void *)114514); /*这里的6也可以设成其它数值*/
}
int main( )
{
	pthread_t tid;
	int res;
	void *temp;
	res=pthread_create(&tid,NULL,create,NULL);
	printf("I am the main thread! \n");
	if(res)
	{
		printf("thread is not created...\n");
		return -1;
	}

	res=pthread_join( tid,&temp);
	if(res)
	{
		printf("Thread is not exit...\n");
		return -2;
	}
	printf("Thread is exit code %d \n" ,(int)temp);
	return 0;
}
