/*thread_join.c*/
#include<pthread.h>
#include<stdio.h>
/*线程函数*/
void *thread(void *str )
{
	int i;
	for(i=0;i<4;++i)
	{
		sleep(2); 
		printf("This is the thread : %d\n" ,i);
	}
	return NULL ;
}
int main( )
{
	pthread_t pth; /*线程ID*/
	int i;
	int ret=pthread_create( &pth ,NULL, thread,(void *)(i));
	pthread_join(pth,NULL );
	printf("main thread restart\n");
	for(i=0;i<3;++i)
	{
		sleep(1); 
		printf("This is the main:%d\n",i);
	}
	return 0;
}