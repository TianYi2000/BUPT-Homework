#include   <stdio.h>
#include   <stdlib.h>
#include   <pthread.h>
#include   <unistd.h>
void* func(void * arg)
{
 pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);           //允许退出线程
 pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,   NULL);   //设置立即取消
 while   (1)
{
  printf("Running...\n");
  sleep(0.1);
}
return   NULL;
}

int   main(int   argc,   char   *argv[])
{
	pthread_t   thrd;
	pthread_attr_t   attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,   PTHREAD_CREATE_DETACHED);

	if   (   pthread_create(&thrd,   &attr,   func,   NULL)   )
	{
		perror( "pthread_create   error ");
		exit(EXIT_FAILURE);
	}
	sleep(0.2);
	if   (   !pthread_cancel(thrd)   )
	{
		printf(   "pthread_cancel   OK\n"   );
	}

	printf("End\n");
	return   0;
}